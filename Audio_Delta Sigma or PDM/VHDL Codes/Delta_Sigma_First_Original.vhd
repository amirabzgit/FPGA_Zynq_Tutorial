library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity sigma_delta_first_original is
    Port ( clk   : in  STD_LOGIC;
           Din     : in  STD_LOGIC_VECTOR (15 downto 0);
           rst : in  STD_LOGIC;
           ce    : in  STD_LOGIC;
           Dout     : out STD_LOGIC);
end sigma_delta_first_original;

architecture Behavioral of sigma_delta_first_original is
    signal dac_out          : std_logic           := '0';
    signal dac_accum        : signed(19 downto 0) := (others => '0');
    signal dac_feedback     : signed(19 downto 0) := (others => '0');
begin
    Dout <= dac_out;
    with dac_out select dac_feedback <= to_signed(-32768,20) when '1',
                                        to_signed( 32768,20) when others;     
dac2_proc: process(clk)
    variable new_val : signed(19 downto 0);
    begin
        if rising_edge(clk) then
           if rst = '0' then
               dac_accum <= (others => '0');
               dac_out <= '0';
           elsif ce = '1' then
               new_val := dac_accum + signed(Din) + dac_feedback;
               if new_val < 0 then
                  dac_out <= '0';
               else
                  dac_out <= '1';
               end if;
               dac_accum <= new_val;
           end if;
       end if;
    end process;

end Behavioral;