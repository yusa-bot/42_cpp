Ford–Johnson algorithm（Merge-Insertion Sort）
整数を昇順ソート

普通のマージソートとは違い、
2個ずつ比較して大きい側だけを先にソートし、小さい側を特殊な順番で二分挿入する

入力
 ↓
2個ずつペアにする
 ↓
各ペアを small / large に分ける
	small = b_i
	large = a_i
 ↓
largeだけを再帰的にFord–Johnsonソート (main chain)
 ↓
smallをpendingに並べる
 ↓
Jacobsthal順でsmallを選ぶ
 ↓
対応するlargeより前を二分探索
 ↓
main chainへ挿入
