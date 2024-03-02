project('Solar', 'cpp', default_options: ['cpp_std=c++17'])

sources = [
    'src/main.cpp',
    'src/commands/info.cpp',
    'src/commands/install.cpp',
    'src/commands/init.cpp',
]

cli11 = dependency('cli11')
rapidjson = dependency('rapidjson')
curl = dependency('libcurl')

executable(
    'solar',
    sources,
    install: true,
    dependencies: [cli11, rapidjson, curl],
)
