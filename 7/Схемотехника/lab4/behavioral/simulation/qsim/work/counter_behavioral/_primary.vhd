library verilog;
use verilog.vl_types.all;
entity counter_behavioral is
    port(
        clk             : in     vl_logic;
        enable          : in     vl_logic;
        reset           : in     vl_logic;
        counter         : out    vl_logic_vector(3 downto 0);
        cout            : out    vl_logic
    );
end counter_behavioral;
