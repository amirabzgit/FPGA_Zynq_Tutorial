

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity top_HDMI is

    Port ( CLK_x : in STD_LOGIC;
           CLK_5x : in STD_LOGIC;
           
           pixelClock : out std_logic;          
           Red      : in std_logic_vector(7 downto 0);
           Green    : in std_logic_vector(7 downto 0);
           Blue     : in std_logic_vector(7 downto 0);
           hSync    : in std_logic;
           vSync    : in std_logic;
           blank    : in std_logic;
      
           data_p    : out  STD_LOGIC_VECTOR(2 downto 0);
           data_n    : out  STD_LOGIC_VECTOR(2 downto 0);
           clk_p          : out    std_logic;
           clk_n          : out    std_logic           
           );
end top_HDMI;

architecture Behavioral of top_HDMI is
   COMPONENT dvid
   PORT(
      clk      : IN std_logic;
      clk_n    : IN std_logic;
      clk_pixel: IN std_logic;
      red_p   : IN std_logic_vector(7 downto 0);
      green_p : IN std_logic_vector(7 downto 0);
      blue_p  : IN std_logic_vector(7 downto 0);
      blank   : IN std_logic;
      hsync   : IN std_logic;
      vsync   : IN std_logic;          
      red_s   : OUT std_logic;
      green_s : OUT std_logic;
      blue_s  : OUT std_logic;
      clock_s : OUT std_logic
      );
   END COMPONENT;



   signal clk_dvi  : std_logic := '0';
   signal clk_dvin : std_logic := '0';
   signal clk_vga  : std_logic := '0';

   signal red_s   : std_logic;
   signal green_s : std_logic;
   signal blue_s  : std_logic;
   signal clock_s : std_logic;
   
begin

      clk_dvi <= CLK_5x;  -- for 640x480@60Hz : 125MHZ
      clk_dvin<= not CLK_5x; -- for 640x480@60Hz : 125MHZ, 180 degree phase shift
      clk_vga <= CLK_x;   -- for 640x480@60Hz : 25MHZ 
      
      pixelClock <= CLK_x ; 

Inst_dvid: dvid PORT MAP(
      clk       => clk_dvi,
      clk_n     => clk_dvin, 
      clk_pixel => clk_vga,
      red_p     => red,
      green_p   => green,
      blue_p    => blue,
      blank     => blank,
      hsync     => hsync,
      vsync     => vsync,
      -- outputs to TMDS drivers
      red_s     => red_s,
      green_s   => green_s,
      blue_s    => blue_s,
      clock_s   => clock_s
   );
   
OBUFDS_blue  : OBUFDS port map ( O  => DATA_P(0), OB => DATA_N(0), I  => blue_s  );
OBUFDS_red   : OBUFDS port map ( O  => DATA_P(1), OB => DATA_N(1), I  => green_s );
OBUFDS_green : OBUFDS port map ( O  => DATA_P(2), OB => DATA_N(2), I  => red_s   );
OBUFDS_clock : OBUFDS port map ( O  => CLK_P, OB => CLK_N, I  => clock_s );
    -- generic map ( IOSTANDARD => "DEFAULT")    
   

end Behavioral;