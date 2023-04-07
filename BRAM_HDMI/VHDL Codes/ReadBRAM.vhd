

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_unsigned.all;

entity ReadBRAM is
    Port ( 
        CLK         : in  STD_LOGIC;
        LEDred      : out STD_LOGIC := '1';
        --Addr_in     : out std_logic_vector(31 downto 0);

        -- To the BRAM        
        CLK_bram    : out STD_LOGIC;
        Addr_bram   : out std_logic_vector(31 downto 0);
        din_bram    : out std_logic_vector(31 downto 0);
        dout_bram   : in  std_logic_vector(31 downto 0);
        EN_bram     : out STD_LOGIC := '0';
        RST_bram    : out STD_LOGIC := '1';
        WEB_bram    : out std_logic_vector(3 downto 0));
end ReadBRAM;

architecture Behavioral of ReadBRAM is
signal	CNTR:	std_logic_vector(31 downto 0) := (others => '0');
signal	DREG:	std_logic_vector(31 downto 0) := (others => '0');
signal  ADDCNTR:std_logic_vector(31 downto 0) := (others => '0');

begin

CLK_bram <= CLK ;

process(CLK)
begin
	if (rising_edge(CLK)) then
		CNTR <= CNTR + "000000000000000000000000000001";
	
	if (CNTR = "0000000") then 
	   LEDred    <= '1' ;
	elsif (CNTR = "0000110") then 
		WEB_bram  <= "0000" ;
		din_bram  <= (others => '0');
		--EN_bram   <= '0';
		RST_bram  <= '0';
		Addr_bram <= "00000000" & "00000000" & "00000000" & "00000000";
	elsif (CNTR = "0001110") then
	   EN_bram   <= '1';
	   ADDCNTR  <= ADDCNTR + "00000001" ;
	   
	elsif (CNTR = "0011110") then --Writing to the BRAM at adress:0b0111
	   Addr_bram <= ADDCNTR(29 downto 0) & "00";
	   din_bram  <= ADDCNTR;
	   WEB_bram  <= "1111" ;
	elsif (CNTR = "0011111") then
	   WEB_bram  <= "0000" ;
	   Addr_bram <= "00000000" & "00000000" & "00000000" & "00000000";
	elsif (CNTR = "00100000" & "00000000" & "00000000" & "00000000") then   
	   Addr_bram <= "00000000" & "00000000" & "00000000" & "00000000";
	elsif (CNTR = "00100000" & "00000000" & "00000000" & "00000001") then	     
       --ADDCNTR  <= ADDCNTR + "00000001" ;   
	   DREG <= dout_bram ; 
	   if (DREG = X"FFFFFFFF") then --"01010101" & "01010101" & "01010101" & "01010101") then
	       LEDred    <= '0' ;
	   else
	       LEDred    <= '1' ;
	   end if;
	elsif (CNTR = "00101000" & "00000000" & "00000000" & "00000001") then 
	   LEDred    <= '1' ;
	end if;   

    end if;
end process;
end Behavioral;
