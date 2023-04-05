
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity FreqDivider is
    Port ( CLK : in STD_LOGIC;
           LED : out STD_LOGIC);
end FreqDivider;

architecture Behavioral of FreqDivider is
    signal  CNTR    :   std_logic_vector(27 downto 0) := (others => '0') ;
begin
process(CLK)
begin
    if (rising_edge(CLK)) then
        CNTR <= CNTR + "000001" ;
    end if;
end process;
LED <= CNTR(26);
end Behavioral;
