import locale
import subprocess

os_encoding = locale.getpreferredencoding()
print(os_encoding)

f = open("sample", 'r')

sample_inputs = f.readlines()
test_case = []
ans = []

for sample in sample_inputs :
    , contents = sample.strip().split()
    
    if (tc == "answer:") :
        ans.append(a)
    else :
        test_case.append(a)

for seq, tc in enumerate(test_case):
    p = subprocess.run(["./a.out"], input=tc.encode(os_encoding), stdout=subprocess.PIPE)

    my_result = p.stdout.decode(os_encoding).strip('\n').strip()
    if (my_result != ans[seq]) :
        print("[{0}]: wrong".format(seq))
        print("my_ans : ", my_result)
        print("correct: ", ans[seq])
    else :
        print("[{0}]: right".format(seq))

f.close()