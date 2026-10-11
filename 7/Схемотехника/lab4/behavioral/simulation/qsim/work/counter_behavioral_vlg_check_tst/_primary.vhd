library verilog;
use verilog.vl_types.all;
entity counter_behavioral_vlg_check_tst is
    port(
        counter         : in     vl_logic_vector(3 downto 0);
        cout            : in     vl_logic;
        sampler_rx      : in     vl_logic
    );
end counter_behavioral_vlg_check_tst;
