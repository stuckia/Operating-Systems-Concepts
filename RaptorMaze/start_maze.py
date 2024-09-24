kernel_module = open('/proc/raptormaze')

maze = kernel_module.readline();
print(maze)

kernel_module.close()
