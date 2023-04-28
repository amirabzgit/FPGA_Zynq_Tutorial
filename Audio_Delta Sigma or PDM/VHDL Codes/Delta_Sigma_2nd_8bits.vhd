
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Delta_Sigma_2nd_8bits is
   Port ( clk   : in  STD_LOGIC;
       Din     : in  STD_LOGIC_VECTOR (7 downto 0);
       rst : in  STD_LOGIC;
       LED  : out   std_logic;
       ce    : in  STD_LOGIC;
       Dout     : out STD_LOGIC);
end Delta_Sigma_2nd_8bits;

architecture Behavioral of Delta_Sigma_2nd_8bits is
    signal RDout          : std_logic           := '0';
    signal Dac1       : signed(11 downto 0) := (others => '0');
    signal Dac2       : signed(11 downto 0) := (others => '0');
    signal FB     : signed(11 downto 0) := (others => '0');

begin

LED <= RDout;

    Dout <= RDout;
    with RDout select FB <= to_signed(-128,12) when '1',
                                        to_signed( 128,12) when others;     
dac2_proc: process(clk)
    begin
        if rising_edge(clk) then 
           if rst = '0' then
              Dac1 <= (others => '0');
              Dac2 <= (others => '0');
              RDout <= '0';
           elsif ce = '1' then
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


