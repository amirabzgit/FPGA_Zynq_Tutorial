

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity BRAM2DS8bits is
   generic (
      AddrWidth  : natural := 16 ;
	  SampleWidth: natural := 8 ;
	  MaxAddr	 : natural := 1000 ;
      
      
   );

    Port ( CLK : in STD_LOGIC;
           CLKData: in STD_LOGIC;
           SDDin :  out STD_LOGIC_VECTOR (SampleWidth-1 downto 0);
           CLKout:  out STD_LOGIC;
           rst   :  out STD_LOGIC;
           CE    :  out STD_LOGIC 
		   -- BRAM interface
		   Addr_bram   : out std_logic_vector(AddrWidth-1 downto 0);
		   EN_bram     : out STD_LOGIC := '0';
		   dout_bram   : in  std_logic_vector(SampleWidth-1 downto 0);
		   RST_bram    : out STD_LOGIC := '1';
		   din_bram    : out std_logic_vector(SampleWidth-1 downto 0):= (others => 'Z');
		   WEB_bram    : out std_logic_vector(3 downto 0);
		   LED         : out STD_LOGIC := '1'
		   );
end BRAM2DS8bits;

architecture Behavioral of BRAM2DS8bits is

	  signal    RadrBRAM  : std_logic_vector(AddrWidth downto 0) := (others => '0');
	  signal    RData     : std_logic_vector(31 downto 0) := (others => '0');
	  signal    MAXBRAM   : std_logic_vector(31 downto 0) := X"0000FFFF" ;
	  
begin

   Addr_bram <= RadrBRAM ;
   RST_bram <= '0' ;
   EN_bram   <= '1' ;
   WEB_bram  <= "0000" ;
   SDDin	<=	RData ;
   
   process(CLK)
   begin
    if rising_edge(CLK) then
	
		if (RadrBRAM < MaxAddr-1) then
			RData <= dout_bram ;
			RadrBRAM <= RadrBRAM + X"01";
			
		else
			RadrBRAM <= (others => '0');
				
		end if;
	end if;
   end process;

end Behavioral;