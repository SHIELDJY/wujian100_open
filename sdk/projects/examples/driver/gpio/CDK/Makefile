.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-gpio - BuildSet ]----------"
	@
	@make -f "wujian100_open-gpio.mk" MakeIntermediateDirs && make -f "wujian100_open-gpio.mk" -j 4
	@echo Executing Post Build commands ...
	@export CDKPath="D:/software/C-Sky/CDK" ProjectPath="E:/Documents/GitHub/wujian100_open/sdk/projects/examples/driver/gpio/CDK" && "E:/Documents/GitHub/wujian100_open/sdk/projects/examples/driver/gpio/CDK/../../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-gpio - BuildSet ]----------"
	@make -f  "wujian100_open-gpio.mk" clean
