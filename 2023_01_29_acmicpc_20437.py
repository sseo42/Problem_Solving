import sys
read = sys.stdin.readline

t = int(read())

for i in range(t) :
    alphabet_idx = [[] for _ in range(26)]
    alphabet_str = read().strip()
    k = int(read())

    for idx, v in enumerate(alphabet_str) :
        alphabet_idx[ord(v) - ord('a')].append(idx)

    min_seq = 10000
    max_seq = 0
    for idx in alphabet_idx :
        if (len(idx) < k) :
            continue
        for i in range(0, len(idx) - k + 1) :
            if (idx[i + k - 1] - idx[i] + 1 < min_seq) :
                min_seq = idx[i + k - 1] - idx[i] + 1
            if (idx[i + k - 1] - idx[i] + 1 > max_seq) :
                max_seq = idx[i + k - 1] - idx[i] + 1
    if (max_seq == 0) :
        print(-1)
    else :
        print(min_seq, max_seq)