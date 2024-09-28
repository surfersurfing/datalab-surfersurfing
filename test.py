from pycparser import c_parser, c_ast, parse_file
import os
import subprocess
import re

fake_libc_include = os.path.join(os.path.dirname(__file__), "fake_libc_include")

ast = parse_file("bits.c", use_cpp=True, cpp_args=["-I", fake_libc_include])


class FunctionAnalyzer(c_ast.NodeVisitor):
    def __init__(self):
        self.operators = []
        self.control_structures = []
        self.has_array = False
        self.function_name = None
        self.type_conversions = []

    def visit_FuncDef(self, node):
        self.function_name = node.decl.name
        self.generic_visit(node)

    def visit_BinaryOp(self, node):
        self.operators.append(node.op)
        self.generic_visit(node)

    def visit_UnaryOp(self, node):
        self.operators.append(node.op)
        self.generic_visit(node)

    def visit_Assignment(self, node):
        self.operators.append(node.op)
        self.generic_visit(node)

    def visit_If(self, node):
        self.control_structures.append("if")
        self.generic_visit(node)

    def visit_While(self, node):
        self.control_structures.append("while")
        self.generic_visit(node)

    def visit_For(self, node):
        self.control_structures.append("for")
        self.generic_visit(node)

    def visit_Switch(self, node):
        self.control_structures.append("switch")
        self.generic_visit(node)

    def visit_Decl(self, node):
        if isinstance(node.type, c_ast.ArrayDecl):
            self.has_array = True
        self.check_type(node.type)
        self.generic_visit(node)

    def visit_Cast(self, node):
        # 收集类型转换
        self.type_conversions.append((node.to_type, node.expr))
        self.generic_visit(node)

    def check_type(self, node):
        if isinstance(node, c_ast.TypeDecl):
            type_name = " ".join(node.type.names)
            self.control_structures.append(type_name)
        elif isinstance(node, (c_ast.PtrDecl, c_ast.ArrayDecl)):
            self.check_type(node.type)
        elif isinstance(node, c_ast.FuncDecl):
            self.check_type(node.type)


class FunctionFinder(c_ast.NodeVisitor):
    def __init__(self, target_func_name):
        self.target_func_name = target_func_name
        self.analyzer = None

    def visit_FuncDef(self, node):
        if node.decl.name == self.target_func_name:
            self.analyzer = FunctionAnalyzer()
            self.analyzer.visit(node)
            raise StopIteration


true_ans = {
    "bitXor": {"rating": 1, "score": 0, "operation": ["~", "&", "int"], "maxop": 7},
    "samesign": {
        "rating": 2,
        "score": 0,
        "operation": [">>", "<<", "!", "^", "&&", "if", "else", "&", "int"],
        "maxop": 12,
    },
    "logtwo": {
        "rating": 4,
        "score": 0,
        "operation": [">", "<", ">>", "<<", "|", "int"],
        "maxop": 25,
    },
    "byteSwap": {
        "rating": 4,
        "score": 0,
        "operation": ["!", "~", "&", "^", "|", "+", "<<", ">>", "int"],
        "maxop": 17,
    },
    "reverse": {
        "rating": 3,
        "score": 0,
        "operation": [
            "<<",
            "|",
            "&",
            "-",
            "+",
            ">>",
            "for",
            "while",
            "!",
            "~",
            "unsigned",
            "int",
        ],
        "maxop": 30,
    },
    "logicalShift": {
        "rating": 3,
        "score": 0,
        "operation": ["!", "~", "&", "^", "|", "+", "<<", ">>", "int"],
        "maxop": 20,
    },
    "leftBitCount": {
        "rating": 4,
        "score": 0,
        "operation": ["!", "~", "&", "^", "|", "+", ">>", "<<", "int"],
        "maxop": 50,
    },
    "float_i2f": {
        "rating": 4,
        "score": 0,
        "operation": [
            "if",
            "else",
            "while",
            "for",
            "&",
            "|",
            "~",
            "+",
            "-",
            "<<",
            ">>",
            "<",
            ">",
            "!",
            "==",
            "unsigned",
            "int",
        ],
        "maxop": 30,
    },
    "floatScale2": {
        "rating": 4,
        "score": 0,
        "operation": [
            "&",
            "<<",
            ">>",
            "|",
            "if",
            ">",
            "<",
            ">=",
            "<=",
            "!",
            "~",
            "else",
            "+",
            "==",
            "int",
            "unsigned",
        ],
        "maxop": 30,
    },
    "float64_f2i": {
        "rating": 3,
        "score": 0,
        "operation": [
            ">>",
            "<<",
            "|",
            "&",
            "~",
            "!",
            "+",
            "-",
            ">",
            "<",
            ">=",
            "<=",
            "if",
            "else",
            "int",
            "unsigned",
        ],
        "maxop": 60,
    },
    "floatPower2": {
        "rating": 4,
        "score": 0,
        "operation": [
            "<",
            ">",
            "<=",
            ">=",
            "<<",
            ">>",
            "+",
            "-",
            "&",
            "|",
            "~",
            "!",
            "if",
            "else",
            "&&",
            "int",
            "unsigned",
        ],
        "maxop": 30,
    },
}


def test_score(function, real_ans):
    finder = FunctionFinder(function)
    try:
        finder.visit(ast)
    except StopIteration:
        pass
    analyzer = finder.analyzer
    function_name = analyzer.function_name
    operators_temp = [i for i in analyzer.operators if i != "="]
    operators = []
    for i in operators_temp:
        if "--" in i:
            operators.append("-")
        elif "++" in i:
            operators.append("+")
        else:
            operators.append(i)
    control_strcutures = analyzer.control_structures
    array_ok = analyzer.has_array
    if array_ok:
        print(f"Has Array Error!,name:{function_name},point:{0}")
        if real_ans[function_name] != 0:
            real_ans["Total"] -= true_ans[function_name]["rating"]
    else:
        flag_operation = 1
        illegal = []
        for i in operators:
            if i != "==":
                i = i.replace("=", "")
            if i not in true_ans[function_name]["operation"]:
                flag_operation = 0
                illegal.append(i)
                break
        for i in control_strcutures:
            if (
                i not in true_ans[function_name]["operation"]
                and i.lower() not in true_ans[function_name]["operation"]
            ):
                illegal.append(i)
                flag_operation = 0
                break
        if flag_operation == 0:
            print(
                f"Has Illegal operation: {illegal}! name: {function_name}, point: {0}"
            )
            if real_ans[function_name] != 0:
                real_ans["Total"] -= true_ans[function_name]["rating"]
        else:
            if len(operators) > true_ans[function_name]["maxop"]:
                print(
                    f"Exceed maxops, your ops are {len(operators)} > max ops {true_ans[function_name]['maxop']}, name:{function_name}, point:{0}"
                )
                if real_ans[function_name] != 0:
                    real_ans["Total"] -= true_ans[function_name]["rating"]
            else:
                if real_ans[function_name] == 0:
                    print(
                        f"Please look at result.txt, you have some errors. name: {function_name}, point: {0}"
                    )
                else:
                    print(
                        f"Pass, great!!! name: {function_name}, point: {real_ans[function_name]}"
                    )


real_ans = {}
with open("result.txt", "w") as f:
    out = subprocess.run(["make", "clean"])
    out = subprocess.Popen(["make"]).wait()
    out = subprocess.Popen(["./btest"], stderr=f, stdout=f).wait()
    with open("result.txt", "r") as rd:
        for i in rd.readlines():
            if "Score" in i:
                continue
            if "Total points" in i:
                matches = re.findall(r"\b\w+\b|\b\d+\b", i)
                real_ans[matches[0]] = int(matches[2])
            elif "ERROR" in i:
                matches = re.findall(r"\b\w+\b|\b\d+\b", i)
                real_ans[matches[2]] = 0
            elif "Gives" in i:
                continue
            else:
                matches = re.findall(r"\b\w+\b|\b\d+\b", i)
                real_ans[matches[-1]] = int(matches[0])
for i in true_ans.keys():
    test_score(i, real_ans)
print(f"Total Points:{ real_ans['Total'] }")


# AutoGrader
with open(".autograder_result", "w") as f:
    f.write(f"{real_ans['Total']}")
