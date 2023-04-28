library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;


entity Delta_Sigma_1st_8bits is
    Port ( clk   : in  STD_LOGIC;
           Din     : in  STD_LOGIC_VECTOR (7 downto 0);
           rst : in  STD_LOGIC;
           ce    : in  STD_LOGIC;
           LED  :   out std_logic;
           Dout     : out STD_LOGIC);
end Delta_Sigma_1st_8bits;

architecture Behavioral of Delta_Sigma_1st_8bits is
    signal RDout          : std_logic           := '0';
    signal Dac        : signed(11 downto 0) := (others => '0');
    signal FB     : signed(11 downto 0) := (others => '0');
begin

    Dout <= RDout;
    LED <=  RDout;
    FB <= X"080" when RDout = '0' else
		  X"F80";     
dac2_proc: process(clk)
    begin
        if rising_edge(clk) then
           if rst = '0' then
               Dac <= (others => '0');
               RDout <= '0';
           elsif ce = '1' then
               if (Dac + signed(Din) + FB) < 0 then
                  RDout <= '0';
               else
                  RDout <= '1';
               end if;
               Dac <= Dac + signed(Din) + FB ;
           end if;
       end if;
    end process;
end Behavioral;
