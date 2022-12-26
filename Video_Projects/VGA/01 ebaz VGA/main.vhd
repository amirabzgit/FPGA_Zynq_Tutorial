

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.numeric_std.all;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity VGAGen is
    Port ( CLKin : in STD_LOGIC;
           LEDG  : out std_logic;
           LEDR  : out std_logic;
           R : out STD_LOGIC;
           G : out STD_LOGIC;
           B : out STD_LOGIC;
           HSync : out STD_LOGIC;
           VSync : out STD_LOGIC);
end VGAGen;

architecture Behavioral of VGAGen is
signal CNTRV    :   std_logic_vector(9 downto 0) := (others =>'0');
signal CNTRH    :   std_logic_vector(9 downto 0) := (others =>'0');
signal CNTRF    :   std_logic_vector(29 downto 0) := (others =>'0');
signal ISSCRN   :   std_logic;
signal RR,GG,BB :   std_logic;
begin


process(CLKin)
begin
if (CLKin'event and CLKin='1') then
    if  (CNTRH < "1100011111") then -- and CNTRV < "1000001101") then
        CNTRH <= CNTRH + "0001" ;
    elsif(CNTRH >= "1100011111" and CNTRV < "1000001100") then
        CNTRV <= CNTRV + "00001" ;
        CNTRH <= "0000000000" ;
    else
        CNTRH <= "0000000000" ;
        CNTRV <= "0000000000" ;
        CNTRF <= CNTRF + "00001" ;
    end if;
end if;
end process;

ISSCRN <= '1' when (CNTRH < "1010110001" and CNTRH > "0000101111" and CNTRV > "0000100000" and CNTRV < "1000000010") else
          '0';

HSync <= '0' when (CNTRH > "1010111111") else
         '1';
VSync <= '0' when (CNTRV > "1000001010") else
         '1';
         
         
RR <= '1' when (CNTRH >  "0000110000" and CNTRH < "0010010100") else
 '0';
 
 
GG <= '1' when (CNTRH >= "0011111000" and CNTRH < "0101011100") else
 '0'; 
 
BB <= '1' when (CNTRH >= "0111000000" and CNTRH < "1000100100") else
 '0';
         
R <= RR and ISSCRN;
G <= GG and ISSCRN;
B <= BB and ISSCRN;
LEDG <= CNTRF(25);
LEDR <= CNTRF(23);

end Behavioral;
