----------------------------------------------------------------------------------
-- Engineer:       Mike Field <hamster@snap.net.nz>
-- Module Name:    ColourTest - Behavioral 
-- Description:    Generates an 640x480 VGA showing all colours
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity vga is
   generic (
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
   );

    Port ( pixelClock : in  STD_LOGIC;
           Red        : out STD_LOGIC_VECTOR (7 downto 0);
           Green      : out STD_LOGIC_VECTOR (7 downto 0);
           Blue       : out STD_LOGIC_VECTOR (7 downto 0);
           hSync      : out STD_LOGIC;
           vSync      : out STD_LOGIC;
           blank      : out STD_LOGIC);
end vga;

architecture Behavioral of vga is

      signal    hCounter : std_logic_vector(11 downto 0) := (others => '0');
      signal    vCounter : std_logic_vector(11 downto 0) := (others => '0');

      signal    Rred      : std_logic_vector(7 downto 0) := (others => '0');
      signal    Rgreen    : std_logic_vector(7 downto 0) := (others => '0');
      signal    Rblue     : std_logic_vector(7 downto 0) := (others => '0');

      signal    RhSync    : std_logic := '0';
      signal    RvSync    : std_logic := '0';
      signal    Rblank    : std_logic := '0';		
 

begin
   -- Assign the outputs
   hSync <= RhSync;
   vSync <= RvSync;
   Red   <= Rred;
   Green <= Rgreen;
   Blue  <= Rblue;
   blank <= Rblank;
   
   process(pixelClock)
   begin

    if rising_edge(pixelClock) then
        if (hCounter < hMaxCount-1) then
            hCounter <= hCounter+1;
        else
            hCounter <= (others => '0');
            if (vCounter < vMaxCount-1) then
                vCounter <= vCounter+1;
            else
                vCounter <= (others => '0');
            end if;
        end if;
 
    

      if hCounter  < hRez and vCounter  < vRez then
        if (hCounter < 213) then
            Rred <= hCounter(7 downto 0);
            Rred <=   "11111111" ;
            Rgreen <= "00000000" ;
            Rblue  <= "00000000" ;
        elsif (hCounter < 426) then
            Rred <=   "00000001" ;
            Rgreen <= "11111111" ;
            Rblue  <= "00000000" ;
        else
            Rred <=   "00000000" ;
            Rgreen <= "00000000" ;
            Rblue  <= "11111111" ;
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
