################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
lwip-1.4.1/src/netif/ethernetif.obj: /home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/src/netif/ethernetif.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv" --include_path="/home/koitt/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/include" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/example/hdk/inc" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/src/include" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/src/include/ipv4" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/ports/hdk/check" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/ports/hdk/include/netif" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/ports/hdk/include" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/ports/hdk/netif" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/ports/hdk" --include_path="/home/koitt/project/IntegrationProject/coffee_machine/mpu/UDPSuccess_recv/lwip-1.4.1/apps/httpserver_raw" --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --endian=big --preproc_with_compile --preproc_dependency="lwip-1.4.1/src/netif/$(basename $(<F)).d_raw" --obj_directory="lwip-1.4.1/src/netif" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


