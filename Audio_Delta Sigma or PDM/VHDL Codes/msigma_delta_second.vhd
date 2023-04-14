library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity msigma_delta_second is
   Port ( clk   : in  STD_LOGIC;
       Din     : in  STD_LOGIC_VECTOR (15 downto 0);
       rst : in  STD_LOGIC;
       ce    : in  STD_LOGIC;
       Dout     : out STD_LOGIC);
end msigma_delta_second;

architecture Behavioral of msigma_delta_second is
    signal RDout          : std_logic           := '0';
    signal Dac1       : signed(19 downto 0) := (others => '0');
    signal Dac2       : signed(19 downto 0) := (others => '0');
    signal FB     : signed(19 downto 0) := (others => '0');
    begin
    Dout <= RDout;
    with RDout select FB <= to_signed(-32768,20) when '1',
                                        to_signed( 32768,20) when others;     
dac2_proc: process(clk)
    --variable new_val1 : signed(19 downto 0);
    --variable new_val2 : signed(19 downto 0);
    begin
        if rising_edge(clk) then 
           if rst = '0' then
              Dac1 <= (others => '0');
              Dac2 <= (others => '0');
              RDout <= '0';
           elsif ce = '1' then
               --new_val1 := Dac1 + signed(Din) + FB;
               --new_val2 := Dac2 + new_val1  + FB;
               if (Dac2 + Dac1 + signed(Din) + FB  + FB) < 0 then
                  RDout <= '0';
               else
                  RDout <= '1';
               end if;
               Dac1 <= Dac1 + signed(Din) + FB;
               Dac2 <= Dac2 + Dac1 + signed(Din) + FB  + FB;
          end if;
       end if;
    end process;


end Behavioral;