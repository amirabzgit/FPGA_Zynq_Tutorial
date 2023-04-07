

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity BRAM2HDMI is
   generic (
      AddrWidth  : natural := 15 ;
      hRez       : natural := 640;	
      hStartSync : natural := 656;
      hEndSync   : natural := 752;
      hMaxCount  : natural := 800;
      hsyncActive : std_logic := '0';
		
      vRez       : natural := 480;
      vStartSync : natural := 490;
      vEndSync   : natural := 492;
      vMaxCount  : natural := 525;
      vsyncActive : std_logic := '1'
------------------------------------------
---- 800 by 600 resolution
------------------------------------------

--      hRez       : natural := 800;	
--      hStartSync : natural := 840;
--      hEndSync   : natural := 968;
--      hMaxCount  : natural := 1056;
--      hsyncActive : std_logic := '0';
		
--      vRez       : natural := 600;
--      vStartSync : natural := 601;
--      vEndSync   : natural := 605;
--      vMaxCount  : natural := 628;
--      vsyncActive : std_logic := '1'      
      
      
   );

    Port ( pixelClock : in  STD_LOGIC;
           Red        : out STD_LOGIC_VECTOR (7 downto 0);
           Green      : out STD_LOGIC_VECTOR (7 downto 0);
           Blue       : out STD_LOGIC_VECTOR (7 downto 0);
           hSync      : out STD_LOGIC;
           vSync      : out STD_LOGIC;
           blank      : out STD_LOGIC;
		   -- BRAM interface
		   Addr_bram   : out std_logic_vector(AddrWidth downto 0);
		   EN_bram     : out STD_LOGIC := '0';
		   dout_bram   : in  std_logic_vector(31 downto 0);
		   RST_bram    : out STD_LOGIC := '1';
		   din_bram    : out std_logic_vector(31 downto 0):= (others => 'Z');
		   WEB_bram    : out std_logic_vector(3 downto 0);
		   LED         : out STD_LOGIC := '1'
		   );
end BRAM2HDMI;

architecture Behavioral of BRAM2HDMI is

      signal    hCounter : std_logic_vector(11 downto 0) := (others => '0');
      signal    vCounter : std_logic_vector(11 downto 0) := (others => '0');

      signal    Rred      : std_logic_vector(7 downto 0) := (others => '0');
      signal    Rgreen    : std_logic_vector(7 downto 0) := (others => '0');
      signal    Rblue     : std_logic_vector(7 downto 0) := (others => '0');

--      signal    RhSync    : std_logic := '0';
--      signal    RvSync    : std_logic := '0';
      signal    Rblank    : std_logic := '0';		
	  
	  signal    RDisp     : std_logic_vector(5 downto 0) := (others => '0');
	  signal    RShft	  : std_logic_vector(63 downto 0) := (others => '0');
	  signal    RMask	  : std_logic_vector(63 downto 0) := (others => '0');
	  
	  signal    DTA 	  : std_logic_vector(31 downto 0) := (others => '0');
	  
	  signal    RadrBRAM  : std_logic_vector(AddrWidth downto 0) := (others => '0');
	  signal    RData     : std_logic_vector(31 downto 0) := (others => '0');
	  signal    hPic      : natural := 100 ;
	  signal    vPic      : natural := 100 ;
	  signal    MAXBRAM   : std_logic_vector(31 downto 0) := X"0000FFFF" ;
	  
	  
	  
	  signal    CNTR : std_logic_vector(30 downto 0) := (others => '0');
	  

begin
   -- Assign the outputs
--   hSync <= RhSync;
--   vSync <= RvSync;
   Red   <= Rred;
   Green <= Rgreen;
   Blue  <= Rblue;
   blank <= Rblank;
   Addr_bram <= RadrBRAM ;
   RST_bram <= '0' ;
   EN_bram   <= '1';
   WEB_bram  <= "0000" ;
   LED <= CNTR(14);
   
   
   process(pixelClock)
   begin
    if rising_edge(pixelClock) then
	
		if (hCounter < hMaxCount-1) then
			
			if (hCounter < hPic) and (vCounter < vPic) then 
			    
			    RDisp <= RShft(63 downto 58);
				if (RMask(25) = '1' and RMask(19) = '0') then
				    RData <= dout_bram;
				    RShft <= RShft(57 downto 0) & "000000" ;
				    RMask <= RMask(57 downto 0) & "000000" ;
				
				elsif (RMask(26) = '1' and RMask(25) = '0') then
				    RShft <= RShft(57 downto 26) & RData ;
				    RMask <= RMask(57 downto 26) & X"FFFFFFFF" ;
				    RadrBRAM <= RadrBRAM + X"04";
				elsif (RMask(28) = '1' and RMask(27) = '0') then
				    RShft <= RShft(57 downto 28) & RData & "00" ;
				    RMask <= RMask(57 downto 28) & X"FFFFFFFF" & "00" ;
				    RadrBRAM <= RadrBRAM + X"04";
				elsif (RMask(30) = '1' and RMask(29) = '0') then
				    RShft <= RShft(57 downto 30) & RData & "0000" ;
				    RMask <= RMask(57 downto 30) & X"FFFFFFFF" & "0000" ;
				    RadrBRAM <= RadrBRAM + X"04";
				elsif (RMask(32) = '1' and RMask(31) = '0') then
				    RShft <= RShft(57 downto 32) & RData & "000000" ;
				    RMask <= RMask(57 downto 32) & X"FFFFFFFF" & "000000" ;
				    RadrBRAM <= RadrBRAM + X"04";

				else
				    RShft <= RShft(57 downto 0) & "000000" ; 
				    RMask <= RMask(57 downto 0) & "000000" ;
				end if;   

				
			elsif(vCounter = vRez) then
				RadrBRAM <= (others => '0');
			elsif(vCounter = vStartSync) then
				RShft <= dout_bram & X"00000000";
				RMask <= X"FFFFFFFF00000000";
			elsif(vCounter = vEndSync) then
				RadrBRAM <= (2 => '1', others => '0');
			end if; 
		    
		    hCounter <= hCounter+1;
		else		
			hCounter <= (others => '0');
			if (vCounter < vMaxCount-1) then
				vCounter <= vCounter+1;
			else
				vCounter <= (others => '0');
				RShft <= RShft(63 downto 32) & dout_bram ;
				--RMask <= X"FFFFFFFFFFFFFFFF";
				RMask <= RMask(63 downto 32) & X"FFFFFFFF" ;
				RadrBRAM <= RadrBRAM + X"004";
			end if;
        end if;	
		
		
		
		
		
		
		
		
      if hCounter  < hRez and vCounter  < vRez then      
        if (hCounter < hPic) and (vCounter < vPic) then
            Rred   <= RDisp(5) & RDisp(4) & "000000" ;
	   	    Rgreen <= RDisp(3) & RDisp(2) & "000000" ;
		    Rblue  <= RDisp(1) & RDisp(0) & "000000" ;
		    
		else
		    Rred   <= "00000000" ;
		    Rgreen <= "00000000" ;
		    Rblue  <= "00000000" ;
		end if;    
            Rblank <= '0';
      else
         Rred   <= (others => '0');
         Rgreen <= (others => '0');
         Rblue  <= (others => '0');
         Rblank <= '1';
      end if;
    end if;  
      
      -- Are we in the hSync pulse?
      if hCounter >= hStartSync and hCounter < hEndSync then
         hSync <= hSyncActive;
      end if;

      -- Are we in the vSync pulse?
      if vCounter >= vStartSync and vCounter < vEndSync then
         vSync <= vSyncActive; 
      end if;
   end process;

end Behavioral;