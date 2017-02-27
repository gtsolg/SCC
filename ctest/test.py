import os

scc = './scc' if os.name == 'posix' else 'scc.exe'


def run_tests(dir):
    print('\tTesting ' + dir + ':')
    files = [f for f in os.listdir(dir) if f.endswith('t.txt')]
    td = os.path.join(os.getcwd(), dir)
    for f in files:
        t = os.path.join(td, f)
        a = t.replace('t.', 'a.')
        if not os.path.isfile(a):
            a = t
        os.system(scc + ' -c_test_expr ' + '\"' + t + '\" \"' + a + '\"')

if __name__ == '__main__':
    run_tests('exp')