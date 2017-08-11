
# LunaticBrain

## Lunatic Brain とは (What's Lunatic Brain?)

有名な難解言語の1つである Brainfuck に命令・機能を追加したインタプリタです．

開発中であり，本readmeに含まれる仕様の大半は実装されていません．

## 命令(Commands)

#### Brainfuck 基本命令

| chr | C-like   | detail |
|:----|:---------|:-------|
| `>` | `++ptr` | |
| `<` | `--ptr` | |
| `+` | `++(*ptr)` | |
| `-` | `--(*ptr)` | |
| `[` | `while(*ptr){` | |
| `]` | `}` | |
| `.` | `putchar(*ptr)` | |
| `,` | `getchar(*ptr)` | |


#### Lunatic Brain 基本命令(basic)
| chr | C-like   | detail |
|:----|:---------|:-------|
| `^` | `stack_push(*ptr)` | |
| `v` | `(*ptr) = stack_pop()` | |
| `$` | `stack_push(ptr)` | |
| `*` | `ptr = stack_pop()` | |
| `i` | `stack_push(code_ptr)` | (?) |
| `!` | `code_ptr = stack_pop()` | (?) |
| `#` | `(*ptr) <<= 1` | |
| `=` | `(*ptr) >>= 1` | |


#### Lunatic Brain 拡張命令(extended)
| chr | C-like   | detail |
|:----|:---------|:-------|
| `~` | `(*ptr) = ~(*ptr)` | |
| `p` | `printf("%d ",*ptr)` | |
| `s` | `scanf("%d",ptr)` | |
| `E` | `printf("[p:%d, c:%d, s:%d]\n", ptr, code_ptr, stack_size())`

## 仕様(spec)

- 各セルは符号付き32bit整数．0で初期化される．
- EOFは-1
- `]`に対応する`[`が存在しない場合，コード先頭にjumpする．
- `[`に対応する`]`が存在しない場合，`(*ptr)`ならば`HALT`(プログラム停止)の効果．
- `code_ptr` が 0 未満ならば，0にセットされる．
- 次の状態が発生した時，runtime error.
    - 範囲外のメモリ領域をアクセスしようとした


## コマンドオプション (option)

- `--help`,`-h`, `--version`,`-v`
- `--unsafe` OutOfRange をチェックしない
- `--optimize0` 最適化しない
- `--alloc <BYTES>` メモリの確保サイズを指定する(default: 202000byte)
- `--brainf` brainfuck基本命令のみ

## ライセンス

- 一通り動くものができ次第


## Example

### cat (brainfuck only)

code
```
,+[-.,+]
```
stdin
```
foo bar baz
```
stdout
```
foo bar baz
```

### How to use stack

code
```
^s[^s]v[pv]
```
stdin
```
1 2 -3 -4 5 0
```
stdout
```
5 -4 -3 2 1
```

### memptr set
code
```
s>,,+[->,+]^*^*.
```
stdin
```
5 abcdefg
```
stdout
```
e
```


### fast set value
code
```
+###+#+#+##+#p
```
stdin
```
```
stdout
```
314
```

### subroutine
todo
