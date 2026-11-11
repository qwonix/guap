cd simulation/modelsim
vlib timing_work
vlog -work timing_work lab6.vo +define+TIMING ../lab6_tb.v
vsim -t ps -L maxii_ver timing_work.lab6_tb
add wave -radix unsigned /lab6_tb/clk
add wave -radix unsigned /lab6_tb/q
run -all
