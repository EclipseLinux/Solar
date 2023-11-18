project('Solar', 'cpp')
sources = run_command('find', '-depth', '-regex', '\.\/src\/.+\.cpp').stdout().strip()

executable('solar', sources, install: true)
