import os, subprocess

scc = os.path.join(os.getcwd(), '..', 'bin', 'unittest.exe')


def run_tests(dir, cmd):
    print('\n\tTesting ' + dir + ':')
    files = [f for f in os.listdir(dir) if f.endswith('t.txt')]
    td = os.path.join(os.getcwd(), dir)
    for f in files:
        t = os.path.join(td, f)
        a = t.replace('t.', 'a.')
        if not os.path.isfile(a):
            a = t
        subprocess.call([scc, cmd, t, a])


if __name__ == '__main__':
	run_tests('exp', '-c_test_expr')
	run_tests('type', '-c_test_type')