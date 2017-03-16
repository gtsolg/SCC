import os, subprocess

scc = os.path.join(os.getcwd(), '..', 'bin', 'scc.exe')


def run_tests(dir):
    print('\tTesting ' + dir + ':')
    files = [f for f in os.listdir(dir) if f.endswith('t.txt')]
    td = os.path.join(os.getcwd(), dir)
    for f in files:
        t = os.path.join(td, f)
        a = t.replace('t.', 'a.')
        if not os.path.isfile(a):
            a = t
        subprocess.call([scc, '-c_test_expr', t, a])


if __name__ == '__main__':
	run_tests('exp')