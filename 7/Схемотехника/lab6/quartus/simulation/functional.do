vlib work
vlog -work work lab6.v simulation/lab6_tb.v
vsim -t ps work.lab6_tb
add wave -radix unsigned /lab6_tb/clk
add wave -radix unsigned /lab6_tb/q
run -all
