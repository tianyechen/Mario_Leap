import subprocess

print("Welcome to Mario!")
mode = None
mode = raw_input("Press 1) for WASD controls \nPress 2) for Gesture v1 controls \nPress 3) for Gesture v2 and Voice controls \n")

choices = ["1","2","3"]
while mode not in choices:
	print("invalid input! Try again")
	mode = raw_input("Press 1) for WASD controls \nPress 2) for Gesture v1 controls \nPress 3) for Gesture v2 and Voice controls \n")

if mode == "1":
    pass
elif mode =="2":
	import gesture1
        gesture1.main()
else:
	import gesture2
        gesture2.main()
