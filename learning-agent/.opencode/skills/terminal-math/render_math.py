#!/usr/bin/env python3
"""终端数学公式渲染器

将 LaTeX 数学表达式转化为终端可读的 Unicode/ASCII art 格式。
纯标准库实现，零外部依赖。

用法:
  python render_math.py --expr "x^2"
  python render_math.py --expr "\frac{a}{b}" --mode block
  python render_math.py --file notes.md --inplace
"""

import argparse
import re
import sys
from dataclasses import dataclass, field
from typing import Optional


# ─── Unicode 映射表 ────────────────────────────────────────

SYMBOLS = {
    "alpha": "α", "beta": "β", "gamma": "γ", "delta": "δ",
    "epsilon": "ε", "varepsilon": "ε", "zeta": "ζ", "eta": "η",
    "theta": "θ", "vartheta": "ϑ", "iota": "ι", "kappa": "κ",
    "lambda": "λ", "mu": "μ", "nu": "ν", "xi": "ξ",
    "pi": "π", "varpi": "ϖ", "rho": "ρ", "sigma": "σ",
    "tau": "τ", "upsilon": "υ", "phi": "φ", "varphi": "ϕ",
    "chi": "χ", "psi": "ψ", "omega": "ω",
    "Gamma": "Γ", "Delta": "Δ", "Theta": "Θ",
    "Lambda": "Λ", "Xi": "Ξ", "Pi": "Π",
    "Sigma": "Σ", "Upsilon": "Υ", "Phi": "Φ",
    "Psi": "Ψ", "Omega": "Ω",
    "infty": "∞", "partial": "∂", "nabla": "∇",
    "pm": "±", "mp": "∓", "times": "×", "div": "÷",
    "cdot": "·", "cdots": "⋯", "ldots": "…", "vdots": "⋮",
    "leq": "≤", "geq": "≥", "neq": "≠", "approx": "≈",
    "equiv": "≡", "sim": "∼", "propto": "∝",
    "subset": "⊂", "supset": "⊃", "subseteq": "⊆",
    "in": "∈", "notin": "∉", "forall": "∀", "exists": "∃",
    "to": "→", "rightarrow": "→", "Rightarrow": "⇒",
    "leftarrow": "←", "Leftarrow": "⇐",
    "leftrightarrow": "↔", "mapsto": "↦",
    "uparrow": "↑", "downarrow": "↓",
    "cup": "∪", "cap": "∩", "oplus": "⊕", "otimes": "⊗",
    "angle": "∠", "parallel": "∥", "perp": "⊥",
    "int": "∫", "iint": "∬", "oint": "∮",
    "sum": "Σ", "prod": "Π",
    "lim": "lim", "max": "max", "min": "min", "sup": "sup", "inf": "inf",
    "sin": "sin", "cos": "cos", "tan": "tan",
    "arcsin": "arcsin", "arccos": "arccos", "arctan": "arctan",
    "sinh": "sinh", "cosh": "cosh", "tanh": "tanh",
    "ln": "ln", "log": "log", "lg": "lg",
    "exp": "exp", "det": "det", "gcd": "gcd",
    "limsup": "limsup", "liminf": "liminf",
    "sqrt": "√",
    " ": " ", "\\": "\\",
}

SUPER_NUMS = str.maketrans("0123456789", "⁰¹²³⁴⁵⁶⁷⁸⁹")
SUB_NUMS = str.maketrans("0123456789", "₀₁₂₃₄₅₆₇₈₉")
SUPER_CHARS = {
    'a': 'ᵃ', 'b': 'ᵇ', 'c': 'ᶜ', 'd': 'ᵈ', 'e': 'ᵉ', 'f': 'ᶠ',
    'g': 'ᵍ', 'h': 'ʰ', 'i': 'ⁱ', 'j': 'ʲ', 'k': 'ᵏ', 'l': 'ˡ',
    'm': 'ᵐ', 'n': 'ⁿ', 'o': 'ᵒ', 'p': 'ᵖ', 'r': 'ʳ', 's': 'ˢ',
    't': 'ᵗ', 'u': 'ᵘ', 'v': 'ᵛ', 'w': 'ʷ', 'x': 'ˣ', 'y': 'ʸ', 'z': 'ᶻ',
    'A': 'ᴬ', 'B': 'ᴮ', 'D': 'ᴰ', 'E': 'ᴱ', 'G': 'ᴳ',
    'H': 'ᴴ', 'I': 'ᴵ', 'J': 'ᴶ', 'K': 'ᴷ', 'L': 'ᴸ',
    'M': 'ᴹ', 'N': 'ᴺ', 'O': 'ᴼ', 'P': 'ᴾ', 'R': 'ᴿ',
    'T': 'ᵀ', 'U': 'ᵁ', 'V': 'ⱽ', 'W': 'ᵂ',
    '+': '⁺', '-': '⁻', '(': '⁽', ')': '⁾', '=': '⁼',
}
SUB_CHARS = {
    'a': 'ₐ', 'e': 'ₑ', 'h': 'ₕ', 'i': 'ᵢ', 'j': 'ⱼ', 'k': 'ₖ',
    'l': 'ₗ', 'm': 'ₘ', 'n': 'ₙ', 'o': 'ₒ', 'p': 'ₚ',
    'r': 'ᵣ', 's': 'ₛ', 't': 'ₜ', 'u': 'ᵤ', 'v': 'ᵥ', 'x': 'ₓ',
    '+': '₊', '-': '₋', '(': '₍', ')': '₎', '=': '₌',
}

ROOT_MAP = {2: "√", 3: "∛", 4: "∜"}


# ─── Token 类型（AST 节点）──────────────────────────────────

@dataclass
class Token:
    def to_text(self, mode="block") -> str:
        raise NotImplementedError


@dataclass
class Text(Token):
    value: str
    def to_text(self, mode="block") -> str:
        return self.value


@dataclass
class Symbol(Token):
    name: str

    def to_text(self, mode="block") -> str:
        return SYMBOLS.get(self.name, self.name)


@dataclass
class Sup(Token):
    base: Token
    exp: Token

    def to_text(self, mode="block") -> str:
        b = self.base.to_text(mode)
        e = self.exp.to_text(mode)
        if len(e) == 1:
            uc = SUPER_CHARS.get(e) or (e.translate(SUPER_NUMS) if e.isdigit() else None)
            if uc:
                return b + uc
        return f"{b}^({e})"


@dataclass
class Sub(Token):
    base: Token
    sub: Token

    def to_text(self, mode="block") -> str:
        b = self.base.to_text(mode)
        s = self.sub.to_text(mode)
        if len(s) == 1:
            uc = SUB_CHARS.get(s) or (s.translate(SUB_NUMS) if s.isdigit() else None)
            if uc:
                return b + uc
        return f"{b}_({s})"


@dataclass
class SubSup(Token):
    base: Token
    sub: Token
    exp: Token

    def to_text(self, mode="block") -> str:
        b = self.base.to_text(mode)
        s = self.sub.to_text(mode)
        e = self.exp.to_text(mode)
        s_u = SUB_CHARS.get(s) or (s.translate(SUB_NUMS) if len(s) == 1 and s.isdigit() else None)
        e_u = SUPER_CHARS.get(e) or (e.translate(SUPER_NUMS) if len(e) == 1 and e.isdigit() else None)
        if s_u and e_u:
            return b + e_u + s_u
        if s_u:
            return f"{b}{s_u}^({e})"
        if e_u:
            return f"{b}_({s}){e_u}"
        return f"{b}_({s})^({e})"


@dataclass
class Frac(Token):
    num: Token
    den: Token

    def to_text(self, mode="block") -> str:
        num_s = self.num.to_text(mode)
        den_s = self.den.to_text(mode)
        if mode == "inline":
            return f"{num_s} / {den_s}"
        w = max(len(num_s), len(den_s))
        line = "─" * w
        return f"{num_s:^{w}}\n{line}\n{den_s:^{w}}"


@dataclass
class Sqrt(Token):
    body: Token
    n: Optional[int] = None

    def to_text(self, mode="block") -> str:
        b = self.body.to_text(mode)
        if self.n is None or self.n == 2:
            return f"√({b})" if len(b) > 1 else f"√{b}"
        if self.n in ROOT_MAP:
            return f"{ROOT_MAP[self.n]}{b}"
        return f"[{self.n}]√{b}"


@dataclass
class Integral(Token):
    lower: Optional[Token]
    upper: Optional[Token]
    body: Token

    def to_text(self, mode="block") -> str:
        lo = self.lower.to_text(mode) if self.lower else ""
        up = self.upper.to_text(mode) if self.upper else ""
        b = self.body.to_text(mode)
        w = max(len(lo), len(up), 0)
        result = []
        if up:
            result.append(f"{up:>{w+2}}")
        result.append(f"∫ {b}")
        if lo:
            result.append(f"{lo:>{w+2}}")
        return "\n".join(result)


@dataclass
class LargeOp(Token):
    name: str
    lower: Optional[Token]
    upper: Optional[Token]
    body: Optional[Token] = None

    def to_text(self, mode="block") -> str:
        symbol = SYMBOLS.get(self.name, self.name)
        lo = self.lower.to_text(mode) if self.lower else ""
        up = self.upper.to_text(mode) if self.upper else ""
        w = max(len(lo), len(up), 0)
        result = []
        if up:
            result.append(f"{up:^{w}}")
        result.append(f"{symbol:^{w}}")
        if lo:
            result.append(f"{lo:^{w}}")
        if self.body:
            result.append(self.body.to_text(mode))
        return "\n".join(result)


@dataclass
class Limit(Token):
    name: str
    cond: Token

    def to_text(self, mode="block") -> str:
        symbol = SYMBOLS.get(self.name, self.name)
        c = self.cond.to_text(mode)
        return f"{symbol} [{c}]"


@dataclass
class Parens(Token):
    body: Token
    left: str = "("
    right: str = ")"

    def to_text(self, mode="block") -> str:
        return self.left + self.body.to_text(mode) + self.right


@dataclass
class Matrix(Token):
    rows: list[list[Token]]

    def to_text(self, mode="block") -> str:
        cell_strs = [[c.to_text(mode) for c in row] for row in self.rows]
        col_widths = [max(len(cell_strs[r][c]) for r in range(len(cell_strs)))
                      for c in range(len(cell_strs[0]))]
        result = []
        for i, row in enumerate(cell_strs):
            cells = [f"{v:^{col_widths[j]}}" for j, v in enumerate(row)]
            if i == 0:
                line = "┌ " + "  ".join(cells) + " ┐"
            elif i == len(cell_strs) - 1:
                line = "└ " + "  ".join(cells) + " ┘"
            else:
                line = "│ " + "  ".join(cells) + " │"
            result.append(line)
        return "\n".join(result)


@dataclass
class Cases(Token):
    pairs: list[tuple[Token, Token]]

    def to_text(self, mode="block") -> str:
        result = []
        for j, (expr, cond) in enumerate(self.pairs):
            prefix = "{ " if j == 0 else "  "
            result.append(f"{prefix}{expr.to_text(mode)},  {cond.to_text(mode)}")
        return "\n".join(result)


# ─── 分词器 ─────────────────────────────────────────────────

def tokenize(s: str) -> list[Token]:
    tokens = []
    i = 0
    n = len(s)

    def skip_spaces():
        nonlocal i
        while i < n and s[i] == ' ':
            i += 1

    def parse_group():
        """解析 {...} 或单个 token"""
        nonlocal i
        skip_spaces()
        if i >= n:
            return Text("")
        if s[i] == '{':
            i += 1
            depth = 1
            start = i
            while i < n and depth > 0:
                if s[i] == '{':
                    depth += 1
                    i += 1
                elif s[i] == '}':
                    depth -= 1
                    if depth > 0:
                        i += 1
                else:
                    i += 1
            inner = tokenize(s[start:i])
            i += 1  # skip closing }
            if len(inner) == 1:
                return inner[0]
            return Text(join_tokens(inner))
        return parse_token()

    def parse_token() -> Token:
        nonlocal i
        skip_spaces()
        if i >= n:
            return Text("")
        if s[i] == '\\':
            i += 1
            cmd = ""
            while i < n and s[i].isalpha():
                cmd += s[i]
                i += 1
            skip_spaces()
            if cmd == "frac":
                num = parse_group()
                den = parse_group()
                return Frac(num, den)
            elif cmd == "sqrt":
                n_val = None
                if i < n and s[i] == '[':
                    i += 1
                    start = i
                    while i < n and s[i] != ']':
                        i += 1
                    n_str = s[start:i].strip()
                    try:
                        n_val = int(n_str)
                    except ValueError:
                        n_val = None
                    i += 1
                body = parse_group()
                return Sqrt(body, n_val)
            elif cmd in ("int", "iint", "oint"):
                lower, upper = parse_limits()
                skip_spaces()
                rest = s[i:]
                rt = tokenize(rest)
                body_str = "".join(t.to_text() for t in rt).strip()
                i = n
                return Integral(lower, upper, Text(body_str) if body_str else Text(""))
            elif cmd in ("sum", "prod"):
                lower, upper = parse_limits()
                skip_spaces()
                rest = s[i:]
                rt = tokenize(rest)
                body_str = "".join(t.to_text() for t in rt).strip()
                i = n
                return LargeOp(cmd, lower, upper, Text(body_str) if body_str else None)
            elif cmd in ("lim", "limsup", "liminf", "max", "min", "sup", "inf"):
                lower, _upper = parse_limits()
                return Limit(cmd, lower or Text(""))
            elif cmd == "left":
                left = ""
                while i < n and s[i] in "()[]{}|.":
                    if s[i] == '(': left = '('
                    elif s[i] == ')': left = ')'
                    elif s[i] == '[': left = '['
                    elif s[i] == ']': left = ']'
                    elif s[i] == '|': left = '|'
                    i += 1
                inner, right = _parse_until_right()
                if right is None:
                    right_map = {'(': ')', '[': ']', '|': '|'}
                    right = right_map.get(left, left)
                inner_left = left if left else ""
                inner_right = right if right else ""
                return Parens(inner, inner_left, inner_right)
            elif cmd == "begin":
                skip_spaces()
                if i < n and s[i] == '{':
                    i += 1
                start = i
                while i < n and s[i] != '}':
                    i += 1
                env = s[start:i]
                i += 1
                if env == "pmatrix" or env == "bmatrix":
                    return parse_matrix(env)
                elif env == "cases":
                    return parse_cases()
                else:
                    return Text("")
            elif cmd == "end":
                while i < n and s[i] != '}':
                    i += 1
                i += 1
                return Text("")
            elif cmd in SYMBOLS:
                return Symbol(cmd)
            else:
                return Text("\\" + cmd)
        elif s[i] == '^':
            i += 1
            exp = parse_group()
            if tokens:
                base = tokens.pop()
                if isinstance(base, Sub):
                    tokens.append(SubSup(base.base, base.sub, exp))
                else:
                    tokens.append(Sup(base, exp))
            return Text("")
        elif s[i] == '_':
            i += 1
            sub = parse_group()
            if tokens:
                base = tokens.pop()
                if isinstance(base, Sup):
                    tokens.append(SubSup(base.base, sub, base.exp))
                else:
                    tokens.append(Sub(base, sub))
            return Text("")
        elif s[i] == '{':
            return parse_group()
        elif s[i] == '}':
            i += 1
            return Text("")
        elif s[i] == '~':
            i += 1
            return Text(" ")
        else:
            val = s[i]
            i += 1
            return Text(val)

    def parse_limits():
        """解析 _lower ^upper 或 ^upper _lower"""
        nonlocal i
        lower = None
        upper = None
        skip_spaces()
        if i < n and s[i] == '_':
            i += 1
            lower = parse_group()
            skip_spaces()
        if i < n and s[i] == '^':
            i += 1
            upper = parse_group()
            skip_spaces()
        if i < n and s[i] == '_' and lower is None:
            i += 1
            lower = parse_group()
        elif i < n and s[i] == '^' and upper is None:
            i += 1
            upper = parse_group()
        return lower, upper

    def _parse_until_right():
        """返回 (inner_token, right_delimiter)  — right_delimiter 为 None 表示未找到 \\right"""
        nonlocal i
        depth = 0
        leftright_depth = 1
        content_start = i
        while i < n:
            if s[i] == '{':
                depth += 1; i += 1
            elif s[i] == '}':
                depth -= 1; i += 1
            elif s[i] == '\\':
                cmd_start = i
                i += 1
                cmd = ""
                while i < n and s[i].isalpha():
                    cmd += s[i]; i += 1
                if cmd == "left":
                    leftright_depth += 1
                elif cmd == "right":
                    leftright_depth -= 1
                    if leftright_depth == 0:
                        rdelim = ""
                        while i < n and s[i] in "()[]{}|.":
                            ch = s[i]
                            if ch == '(': rdelim = ')'
                            elif ch == ')': rdelim = ')'
                            elif ch == '[': rdelim = ']'
                            elif ch == ']': rdelim = ']'
                            elif ch == '|': rdelim = '|'
                            i += 1
                        inner = s[content_start:cmd_start].strip()
                        inner_tokens = tokenize(inner)
                        if inner_tokens and len(inner_tokens) == 1:
                            return inner_tokens[0], rdelim if rdelim else None
                        return Text(join_tokens(inner_tokens)), rdelim if rdelim else None
            else:
                i += 1
        inner = s[content_start:].strip()
        inner_tokens = tokenize(inner)
        if inner_tokens and len(inner_tokens) == 1:
            return inner_tokens[0], None
        return join_tt(inner_tokens), None

    def parse_matrix(env):
        nonlocal i
        rows = []
        current_row = []
        while i < n:
            skip_spaces()
            if i >= n:
                break
            if s[i] == '\\':
                if i + 1 < n and s[i + 1] == '\\':
                    i += 2
                    rows.append(current_row)
                    current_row = []
                    continue
                i += 1
                cmd = ""
                while i < n and s[i].isalpha():
                    cmd += s[i]
                    i += 1
                if cmd == "end":
                    while i < n and s[i] != '}':
                        i += 1
                    i += 1
                    break
                rows.append(current_row)
                current_row = []
                continue
            elif s[i] == '&':
                i += 1
                continue
            else:
                t = parse_token()
                if not (isinstance(t, Text) and t.value == ""):
                    current_row.append(t)
        if current_row:
            rows.append(current_row)
        return Matrix(rows)

    def parse_cases():
        nonlocal i
        pairs = []
        while i < n:
            skip_spaces()
            if i >= n:
                break
            if s[i] == '\\':
                saved = i
                i += 1
                cmd = ""
                while i < n and s[i].isalpha():
                    cmd += s[i]; i += 1
                if cmd == "end":
                    while i < n and s[i] != '}':
                        i += 1
                    i += 1
                    break
                else:
                    i = saved
            expr_start = i
            while i < n and s[i] != '&' and not (s[i] == '\\' and i + 1 < n and s[i + 1] == '\\'):
                if s[i] == '\\':
                    i += 1
                    while i < n and s[i].isalpha():
                        i += 1
                else:
                    i += 1
            expr_s = s[expr_start:i].strip()
            expr_tokens = tokenize(expr_s)
            if i < n and s[i] == '&':
                i += 1
            cond_start = i
            while i < n and not (s[i] == '\\' and i + 1 < n and s[i + 1] == '\\') \
                      and not (s[i] == '\\' and i + 1 < n and i + 3 < n and s[i:i + 5] == "\\end{"):
                if s[i] == '\\':
                    i += 1
                    while i < n and s[i].isalpha():
                        i += 1
                else:
                    i += 1
            cond_s = s[cond_start:i].strip()
            cond_tokens = tokenize(cond_s)
            expr_token = Text(join_tokens(expr_tokens)) if expr_tokens else Text("")
            cond_token = Text(join_tokens(cond_tokens)) if cond_tokens else Text("")
            pairs.append((expr_token, cond_token))
            if i < n and s[i:i + 2] == '\\\\':
                i += 2
        return Cases(pairs)

    while i < n:
        t = parse_token()
        if not (isinstance(t, Text) and t.value == ""):
            tokens.append(t)

    return tokens


def join_tokens(tokens):
    return "".join(t.to_text() for t in tokens)


def join_tt(tokens):
    s = ""
    for tk in tokens:
        if isinstance(tk, (Text, Symbol)):
            s += tk.to_text()
    return Text(s)


# ─── 渲染入口 ────────────────────────────────────────────────

def _has_latex(text: str) -> bool:
    """检查文本是否包含 LaTeX 控制字符"""
    return any(c in text for c in '\\_^{}&~$%')


def render_latex(latex: str, mode: str = "inline") -> str:
    """
    渲染 LaTeX 表达式为终端可读文本。

    Args:
        latex: LaTeX 表达式（不含 $ 符号）
        mode: "inline" 单行渲染, "block" 块级渲染

    Returns:
        渲染后的字符串
    """
    if not _has_latex(latex):
        return latex
    try:
        tokens = tokenize(latex)
        result = "".join(t.to_text(mode) for t in tokens)
        return result
    except Exception as e:
        return f"${latex}$"


def process_markdown(text: str) -> str:
    """处理 markdown 文本中的 LaTeX 公式"""

    def replace_block(m):
        latex = m.group(1)
        rendered = render_latex(latex, "block")
        if "\n" in rendered:
            return f"```\n{rendered}\n```\n<!-- $$ {latex} $$ -->"
        return rendered

    def replace_inline(m):
        latex = m.group(1)
        rendered = render_latex(latex, "inline")
        if "\n" in rendered:
            return f"\n```\n{rendered}\n```\n"
        return rendered

    # 处理 $$...$$ 块
    text = re.sub(r'\$\$(.+?)\$\$', replace_block, text, flags=re.DOTALL)
    # 处理 $...$ 内联
    text = re.sub(r'\$(.+?)\$', replace_inline, text)
    return text


# ─── CLI ─────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="终端数学公式渲染器——LaTeX → Unicode/ASCII art"
    )
    parser.add_argument(
        "--expr", default=None,
        help="LaTeX 表达式（不含 $ 符号）"
    )
    parser.add_argument(
        "--mode", default="inline", choices=["inline", "block"],
        help="inline=单行渲染, block=块级渲染 (默认: inline)"
    )
    parser.add_argument(
        "--file", default=None,
        help="要处理的 markdown 文件路径"
    )
    parser.add_argument(
        "--inplace", action="store_true",
        help="就地替换文件内容"
    )

    args = parser.parse_args()

    if args.file:
        with open(args.file, "r", encoding="utf-8") as f:
            content = f.read()
        result = process_markdown(content)
        if args.inplace:
            with open(args.file, "w", encoding="utf-8") as f:
                f.write(result)
        else:
            print(result)
    elif args.expr:
        result = render_latex(args.expr, args.mode)
        print(result)
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
