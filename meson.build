project('Solar', 'cpp')
sources = [
    'src/main.cpp', 
    'src/commands/info.cpp', 
    'src/commands/install.cpp', 
    'src/commands/init.cpp'
]

cli11 = dependency('cli11')
rapidjson = dependency('rapidjson')

executable(
    'solar',
    sources,
    install: true,
    dependencies: [cli11, rapidjson],
)
