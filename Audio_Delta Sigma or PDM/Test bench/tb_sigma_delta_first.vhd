library ieee;
use ieee.std_logic_1164.all;

entity tb_sigma_delta_first is
end tb_sigma_delta_first;

architecture tb of tb_sigma_delta_first is

    component sigma_delta_first
        port (clk  : in std_logic;
              Din  : in std_logic_vector (15 downto 0);
              rst  : in std_logic;
              CE   : in std_logic;
              Dout : out std_logic);
    end component;
    
    component msigma_delta_first
        Port ( clk   : in  STD_LOGIC;
           Din   : in  STD_LOGIC_VECTOR (15 downto 0);
           rst	 : in  STD_LOGIC;
		   CE	 : in  STD_LOGIC;
           Dout     : out STD_LOGIC);
    end component;   

    signal clk  : std_logic;
    signal Din1K  : std_logic_vector (15 downto 0);
    signal Din600 : std_logic_vector (15 downto 0);
    signal rst  : std_logic;
    signal CE   : std_logic;
    signal Dout : std_logic;
    signal mDout1K : std_logic;
    signal mDout600 : std_logic;
    signal clk_EN  : boolean := false;
    constant TbPeriod : time := 1000 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';

begin

    dut : sigma_delta_first
    port map (clk  => clk,
              Din  => Din1K,
              rst  => rst,
              CE   => CE,
              Dout => Dout);
    mdut : msigma_delta_first
    port map (clk  => clk,
              Din  => Din1K,
              rst  => rst,
              CE   => CE,
              Dout => mDout1K);          
     mdut600 : msigma_delta_first
    port map (clk  => clk,
              Din  => Din600,
              rst  => rst,
              CE   => CE,
              Dout => mDout600);          
             

    -- Clock generation
    TbClock <= not TbClock after TbPeriod/8 when CLK_en else  '0' ;  -- not TbClock after TbPeriod/2 ;
    clk <= TbClock;
	CE <= '1';
	rst <= '0', '1' after 5*TbPeriod;


stimuli2 : process
    begin
        wait for TbPeriod/2;
        CLK_en <= true;

    end process;
    
    stimuli : process
    begin
        -- EDIT Adapt initialization as needed
        while true loop
        Din1K <= X"0000";
        wait for TbPeriod;
        Din1K <= X"1237";
        wait for TbPeriod;
        Din1K <= X"240F";
        wait for TbPeriod;
        Din1K <= X"352C";
        wait for TbPeriod;
        Din1K <= X"4533";
        wait for TbPeriod;
        Din1K <= X"53D2";
        wait for TbPeriod;
        Din1K <= X"60BC";
        wait for TbPeriod;
        Din1K <= X"6BAE";
        wait for TbPeriod;
        Din1K <= X"746E";
        wait for TbPeriod;
        Din1K <= X"7AD0";
        wait for TbPeriod;
        Din1K <= X"7EB2";
        wait for TbPeriod;
        Din1K <= X"8000";
        wait for TbPeriod;
        Din1K <= X"7EB2";
        wait for TbPeriod;
        Din1K <= X"7AD0";
        wait for TbPeriod;
        Din1K <= X"6BAE";
        wait for TbPeriod;
        Din1K <= X"60BC";
        wait for TbPeriod;
        Din1K <= X"53D2";
        wait for TbPeriod;
        Din1K <= X"4533";
        wait for TbPeriod;
        Din1K <= X"352C";
        wait for TbPeriod;
        Din1K <= X"240F";
        wait for TbPeriod;
        Din1K <= X"1237";
        wait for TbPeriod;
        Din1K <= X"0000";
        wait for TbPeriod;
        Din1K <= X"EDC9";
        wait for TbPeriod;    
        Din1K <= X"DBF1";
        wait for TbPeriod;        
        Din1K <= X"CAD4";
        wait for TbPeriod;
        Din1K <= X"BACD";
        wait for TbPeriod;
        Din1K <= X"AC2E";
        wait for TbPeriod;
        Din1K <= X"9F44";
        wait for TbPeriod;
        Din1K <= X"9452";
        wait for TbPeriod;
        Din1K <= X"8B92";
        wait for TbPeriod;
        Din1K <= X"8530";
        wait for TbPeriod;
        Din1K <= X"814E";
        wait for TbPeriod;
        Din1K <= X"8000";
        wait for TbPeriod;
        Din1K <= X"814E";
        wait for TbPeriod;
        Din1K <= X"8530";
        wait for TbPeriod;
        Din1K <= X"8B92";
        wait for TbPeriod;
        Din1K <= X"9452";
        wait for TbPeriod;
        Din1K <= X"9F44";
        wait for TbPeriod;
        Din1K <= X"AC2E";
        wait for TbPeriod;
        Din1K <= X"BACD";
        wait for TbPeriod;
        Din1K <= X"CAD4";
        wait for TbPeriod;
        Din1K <= X"DBF1";
        wait for TbPeriod;
        Din1K <= X"EDC9";
        wait for TbPeriod;
        
        end loop;
        
        wait;
    end process;



    stimuli600 : process
    begin
        -- EDIT Adapt initialization as needed
        while true loop

		Din600 <= X"0000"; 
        wait for TbPeriod;
        Din600 <= X"0AF4"; 
        wait for TbPeriod;
        Din600 <= X"15D3"; 
        wait for TbPeriod;
        Din600 <= X"208A"; 
        wait for TbPeriod;
        Din600 <= X"2B03"; 
        wait for TbPeriod;
        Din600 <= X"352C"; 
        wait for TbPeriod;
        Din600 <= X"3EF1"; 
        wait for TbPeriod;
        Din600 <= X"483F"; 
        wait for TbPeriod;
        Din600 <= X"5106"; 
        wait for TbPeriod;
        Din600 <= X"5935"; 
        wait for TbPeriod;
        Din600 <= X"60BC"; 
        wait for TbPeriod;
        Din600 <= X"678D"; 
        wait for TbPeriod;
        Din600 <= X"6D9C"; 
        wait for TbPeriod;
        Din600 <= X"72DD"; 
        wait for TbPeriod;
        Din600 <= X"7747"; 
        wait for TbPeriod;
        Din600 <= X"7AD0"; 
        wait for TbPeriod;
        Din600 <= X"7D73"; 
        wait for TbPeriod;
        Din600 <= X"7F2A"; 
        wait for TbPeriod;
        Din600 <= X"7FF2"; 
        wait for TbPeriod;
        Din600 <= X"7FCA"; 
        wait for TbPeriod;
        Din600 <= X"7EB2"; 
        wait for TbPeriod;
        Din600 <= X"7CAC"; 
        wait for TbPeriod;
        Din600 <= X"79BC"; 
        wait for TbPeriod;
        Din600 <= X"75E7"; 
        wait for TbPeriod;
        Din600 <= X"7135"; 
        wait for TbPeriod;
        Din600 <= X"6BAE"; 
        wait for TbPeriod;
        Din600 <= X"655D"; 
        wait for TbPeriod;
        Din600 <= X"5E4D"; 
        wait for TbPeriod;
        Din600 <= X"568C"; 
        wait for TbPeriod;
        Din600 <= X"4E29"; 
        wait for TbPeriod;
        Din600 <= X"4533"; 
        wait for TbPeriod;
        Din600 <= X"3BBB"; 
        wait for TbPeriod;
        Din600 <= X"31D3"; 
        wait for TbPeriod;
        Din600 <= X"278D"; 
        wait for TbPeriod;
        Din600 <= X"1CFD"; 
        wait for TbPeriod;
        Din600 <= X"1237"; 
        wait for TbPeriod;
        Din600 <= X"074E"; 
        wait for TbPeriod;
        Din600 <= X"FC59"; 
        wait for TbPeriod;
        Din600 <= X"F169"; 
        wait for TbPeriod;
        Din600 <= X"E695"; 
        wait for TbPeriod;
        Din600 <= X"DBF1"; 
        wait for TbPeriod;
        Din600 <= X"D190"; 
        wait for TbPeriod;
        Din600 <= X"C787"; 
        wait for TbPeriod;
        Din600 <= X"BDE7"; 
        wait for TbPeriod;
        Din600 <= X"B4C4"; 
        wait for TbPeriod;
        Din600 <= X"AC2E"; 
        wait for TbPeriod;
        Din600 <= X"A436"; 
        wait for TbPeriod;
        Din600 <= X"9CE9"; 
        wait for TbPeriod;
        Din600 <= X"9657"; 
        wait for TbPeriod;
        Din600 <= X"908C"; 
        wait for TbPeriod;
        Din600 <= X"8B92"; 
        wait for TbPeriod;
        Din600 <= X"8772"; 
        wait for TbPeriod;
        Din600 <= X"8435"; 
        wait for TbPeriod;
        Din600 <= X"81E0"; 
        wait for TbPeriod;
        Din600 <= X"8079"; 
        wait for TbPeriod;
        Din600 <= X"8000"; 
        wait for TbPeriod;
        Din600 <= X"8079"; 
        wait for TbPeriod;
        Din600 <= X"81E0"; 
        wait for TbPeriod;
        Din600 <= X"8435"; 
        wait for TbPeriod;
        Din600 <= X"8772"; 
        wait for TbPeriod;
        Din600 <= X"8B92"; 
        wait for TbPeriod;
        Din600 <= X"908C"; 
        wait for TbPeriod;
        Din600 <= X"9657"; 
        wait for TbPeriod;
        Din600 <= X"9CE9"; 
        wait for TbPeriod;
        Din600 <= X"A436"; 
        wait for TbPeriod;
        Din600 <= X"AC2E"; 
        wait for TbPeriod;
        Din600 <= X"B4C4"; 
        wait for TbPeriod;
        Din600 <= X"BDE7"; 
        wait for TbPeriod;
        Din600 <= X"C787"; 
        wait for TbPeriod;
        Din600 <= X"D190"; 
		wait for TbPeriod;
        Din600 <= X"DBF1"; 
        wait for TbPeriod;
        Din600 <= X"E695"; 
        wait for TbPeriod;
        Din600 <= X"F169"; 
        wait for TbPeriod;
        Din600 <= X"FC59"; 
        wait for TbPeriod;
        Din600 <= X"074E"; 
        wait for TbPeriod;
        Din600 <= X"1237"; 
        wait for TbPeriod;
        Din600 <= X"1CFD"; 
        wait for TbPeriod;
        Din600 <= X"278D"; 
        wait for TbPeriod;
        Din600 <= X"31D3"; 
        wait for TbPeriod;
        Din600 <= X"3BBB"; 
        wait for TbPeriod;
        Din600 <= X"4533"; 
        wait for TbPeriod;
        Din600 <= X"4E29"; 
        wait for TbPeriod;
        Din600 <= X"568C"; 
        wait for TbPeriod;
        Din600 <= X"5E4D"; 
        wait for TbPeriod;
        Din600 <= X"655D"; 
        wait for TbPeriod;
        Din600 <= X"6BAE"; 
        wait for TbPeriod;
        Din600 <= X"7135"; 
        wait for TbPeriod;
        Din600 <= X"75E7"; 
        wait for TbPeriod;
        Din600 <= X"79BC"; 
        wait for TbPeriod;
        Din600 <= X"7CAC"; 
        wait for TbPeriod;
        Din600 <= X"7EB2"; 
        wait for TbPeriod;
        Din600 <= X"7FCA"; 
        wait for TbPeriod;
        Din600 <= X"7FF2"; 
        wait for TbPeriod;
        Din600 <= X"7F2A"; 
        wait for TbPeriod;
        Din600 <= X"7D73"; 
        wait for TbPeriod;
        Din600 <= X"7AD0"; 
        wait for TbPeriod;
        Din600 <= X"7747"; 
        wait for TbPeriod;
        Din600 <= X"72DD"; 
        wait for TbPeriod;
        Din600 <= X"6D9C"; 
        wait for TbPeriod;
        Din600 <= X"678D"; 
        wait for TbPeriod;
        Din600 <= X"60BC"; 
        wait for TbPeriod;
        Din600 <= X"5935"; 
        wait for TbPeriod;
        Din600 <= X"5106"; 
        wait for TbPeriod;
        Din600 <= X"483F"; 
        wait for TbPeriod;
        Din600 <= X"3EF1"; 
        wait for TbPeriod;
        Din600 <= X"352C"; 
        wait for TbPeriod;
        Din600 <= X"2B03"; 
        wait for TbPeriod;
        Din600 <= X"208A"; 
        wait for TbPeriod;
        Din600 <= X"15D3"; 
        wait for TbPeriod;
        Din600 <= X"0AF4"; 
        wait for TbPeriod;
        Din600 <= X"0000"; 
        wait for TbPeriod;
        Din600 <= X"F50C"; 
        wait for TbPeriod;
        Din600 <= X"EA2D"; 
        wait for TbPeriod;
        Din600 <= X"DF76"; 
        wait for TbPeriod;
        Din600 <= X"D4FD"; 
        wait for TbPeriod;
        Din600 <= X"CAD4"; 
        wait for TbPeriod;
        Din600 <= X"C10F"; 
        wait for TbPeriod;
        Din600 <= X"B7C1"; 
        wait for TbPeriod;
        Din600 <= X"AEFA"; 
        wait for TbPeriod;
        Din600 <= X"A6CB"; 
        wait for TbPeriod;
        Din600 <= X"9F44"; 
        wait for TbPeriod;
        Din600 <= X"9873"; 
        wait for TbPeriod;
        Din600 <= X"9264"; 
        wait for TbPeriod;
        Din600 <= X"8D23"; 
        wait for TbPeriod;
        Din600 <= X"88B9"; 
        wait for TbPeriod;
        Din600 <= X"8530"; 
        wait for TbPeriod;
        Din600 <= X"828D"; 
        wait for TbPeriod;
        Din600 <= X"80D6"; 
        wait for TbPeriod;
        Din600 <= X"800E"; 
        wait for TbPeriod;
        Din600 <= X"8036"; 
        wait for TbPeriod;
        Din600 <= X"814E"; 
        wait for TbPeriod;
        Din600 <= X"8354"; 
        wait for TbPeriod;
        Din600 <= X"8644"; 
        wait for TbPeriod;
        Din600 <= X"8A19"; 
        wait for TbPeriod;
        Din600 <= X"8ECB"; 
        wait for TbPeriod;
        Din600 <= X"9452"; 
        wait for TbPeriod;
        Din600 <= X"9AA3"; 
        wait for TbPeriod;
        Din600 <= X"A1B3"; 
        wait for TbPeriod;
        Din600 <= X"A974"; 
        wait for TbPeriod;
        Din600 <= X"B1D7"; 
        wait for TbPeriod;
        Din600 <= X"BACD"; 
        wait for TbPeriod;
        Din600 <= X"C445"; 
        wait for TbPeriod;
        Din600 <= X"CE2D"; 
        wait for TbPeriod;
        Din600 <= X"D873"; 
        wait for TbPeriod;
        Din600 <= X"E303"; 
        wait for TbPeriod;
        Din600 <= X"EDC9"; 
        wait for TbPeriod;
        Din600 <= X"F8B2"; 
        wait for TbPeriod;
        Din600 <= X"03A7"; 
        wait for TbPeriod;
        Din600 <= X"0E97"; 
        wait for TbPeriod;
        Din600 <= X"196B"; 
        wait for TbPeriod;
        Din600 <= X"240F"; 
        wait for TbPeriod;
        Din600 <= X"2E70"; 
        wait for TbPeriod;
        Din600 <= X"3879"; 
        wait for TbPeriod;
        Din600 <= X"4219"; 
        wait for TbPeriod;
        Din600 <= X"4B3C"; 
        wait for TbPeriod;
        Din600 <= X"53D2"; 
        wait for TbPeriod;
        Din600 <= X"5BCA"; 
        wait for TbPeriod;
        Din600 <= X"6317"; 
        wait for TbPeriod;
        Din600 <= X"69A9"; 
        wait for TbPeriod;
        Din600 <= X"6F74"; 
        wait for TbPeriod;
        Din600 <= X"746E"; 
        wait for TbPeriod;
        Din600 <= X"788E"; 
        wait for TbPeriod;
        Din600 <= X"7BCB"; 
        wait for TbPeriod;
        Din600 <= X"7E20"; 
        wait for TbPeriod;
        Din600 <= X"7F87"; 
        wait for TbPeriod;
        Din600 <= X"8000"; 
        wait for TbPeriod;
        Din600 <= X"7F87"; 
        wait for TbPeriod;
        Din600 <= X"7E20"; 
        wait for TbPeriod;
        Din600 <= X"7BCB"; 
        wait for TbPeriod;
        Din600 <= X"788E"; 
        wait for TbPeriod;
        Din600 <= X"746E"; 
        wait for TbPeriod;
        Din600 <= X"6F74"; 
        wait for TbPeriod;
        Din600 <= X"69A9"; 
        wait for TbPeriod;
        Din600 <= X"6317"; 
        wait for TbPeriod;
        Din600 <= X"5BCA"; 
        wait for TbPeriod;
        Din600 <= X"53D2"; 
        wait for TbPeriod;
        Din600 <= X"4B3C"; 
        wait for TbPeriod;
        Din600 <= X"4219"; 
        wait for TbPeriod;
        Din600 <= X"3879"; 
        wait for TbPeriod;
        Din600 <= X"2E70"; 
        wait for TbPeriod;
        Din600 <= X"240F"; 
        wait for TbPeriod;
        Din600 <= X"196B"; 
        wait for TbPeriod;
        Din600 <= X"0E97"; 
        wait for TbPeriod;
        Din600 <= X"03A7"; 
        wait for TbPeriod;
        Din600 <= X"F8B2"; 
        wait for TbPeriod;
        Din600 <= X"EDC9"; 
        wait for TbPeriod;
        Din600 <= X"E303"; 
        wait for TbPeriod;
        Din600 <= X"D873"; 
        wait for TbPeriod;
        Din600 <= X"CE2D"; 
        wait for TbPeriod;
        Din600 <= X"C445"; 
        wait for TbPeriod;
        Din600 <= X"BACD"; 
        wait for TbPeriod;
        Din600 <= X"B1D7"; 
        wait for TbPeriod;
        Din600 <= X"A974"; 
        wait for TbPeriod;
        Din600 <= X"A1B3"; 
        wait for TbPeriod;
        Din600 <= X"9AA3"; 
        wait for TbPeriod;
        Din600 <= X"9452"; 
        wait for TbPeriod;
        Din600 <= X"8ECB"; 
        wait for TbPeriod;
        Din600 <= X"8A19"; 
        wait for TbPeriod;
        Din600 <= X"8644"; 
        wait for TbPeriod;
        Din600 <= X"8354"; 
        wait for TbPeriod;
        Din600 <= X"814E"; 
        wait for TbPeriod;
        Din600 <= X"8036"; 
        wait for TbPeriod;
        Din600 <= X"800E"; 
        wait for TbPeriod;
        Din600 <= X"80D6"; 
        wait for TbPeriod;
        Din600 <= X"828D"; 
        wait for TbPeriod;
        Din600 <= X"8530"; 
        wait for TbPeriod;
        Din600 <= X"88B9"; 
        wait for TbPeriod;
        Din600 <= X"8D23"; 
        wait for TbPeriod;
        Din600 <= X"9264"; 
        wait for TbPeriod;
        Din600 <= X"9873"; 
        wait for TbPeriod;
        Din600 <= X"9F44"; 
        wait for TbPeriod;
        Din600 <= X"A6CB"; 
        wait for TbPeriod;
        Din600 <= X"AEFA"; 
        wait for TbPeriod;
        Din600 <= X"B7C1"; 
        wait for TbPeriod;
        Din600 <= X"C10F"; 
        wait for TbPeriod;
        Din600 <= X"CAD4"; 
        wait for TbPeriod;
        Din600 <= X"D4FD"; 
        wait for TbPeriod;
        Din600 <= X"DF76"; 
        wait for TbPeriod;
        Din600 <= X"EA2D"; 
        wait for TbPeriod;
        Din600 <= X"F50C"; 
        wait for TbPeriod;
        
        end loop;
        
        wait;
    end process;



end tb;