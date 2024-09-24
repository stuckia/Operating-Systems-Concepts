kernel_module = open('/proc/raptormaze')

maze = kernel_module.read()
print(maze)

kernel_module.close()
