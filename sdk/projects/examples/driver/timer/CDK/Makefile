.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-timer - BuildSet ]----------"
	@
	@make -f "wujian100_open-timer.mk" MakeIntermediateDirs && make -f "wujian100_open-timer.mk" -j 4
	@echo Executing Post Build commands ...
	@export CDKPath="D:/software/C-Sky/CDK" ProjectPath="E:/Documents/GitHub/wujian100_open/sdk/projects/examples/driver/timer/CDK" && "E:/Documents/GitHub/wujian100_open/sdk/projects/examples/driver/timer/CDK/../../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-timer - BuildSet ]----------"
	@make -f  "wujian100_open-timer.mk" clean
