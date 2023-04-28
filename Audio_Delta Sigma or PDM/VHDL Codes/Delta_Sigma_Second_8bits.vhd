library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_signed.all;

entity Delta_Sigma_Second_8bits is
generic (
	  DataWidth: natural := 8    
   );

   Port ( clk   : in  STD_LOGIC;
       Din     : in  STD_LOGIC_VECTOR (DataWidth-1 downto 0);
       rst : in  STD_LOGIC;
       ce    : in  STD_LOGIC;
       LED  :   out std_logic;
       Dout     : out STD_LOGIC);

end Delta_Sigma_Second_8bits;

architecture Behavioral of Delta_Sigma_Second_8bits is
    signal RDout          : std_logic           := '0';
    signal Dac1       : signed(DataWidth+3 downto 0) := (others => '0');
    signal Dac2       : signed(DataWidth+3 downto 0) := (others => '0');
    signal FB     : signed(DataWidth+3 downto 0) := (others => '0');
    signal    NegValue    : std_logic_vector(DataWidth+3 downto 0) ; 
    signal    PosValue    : std_logic_vector(DataWidth+3 downto 0) ; 
    signal    midValue    : std_logic_vector(DataWidth+3 downto 0) ;

begin
    midValue <= (not PosValue) ;
    NegValue <= midValue + X"01";
    PosValue <= (DataWidth-1 => '1', others => '0') ;
    Dout <= RDout;
    LED <= RDout;
    
    with RDout select FB <= to_signed(-128,DataWidth+4) when '1',
                                        to_signed( 128,DataWidth+4) when others; 


--    with RDout select FB <= signed(NegValue) when '1',
--                        signed(PosValue) when others;                                             
                                            
dac2_proc: process(clk)
    begin
        if rising_edge(clk) then 
           if rst = '0' then
              Dac1 <= (others => '0');
              Dac2 <= (others => '0');
              RDout <= '0';
           elsif ce = '1' then
               --new_val1 := Dac1 + signed(Din) + FB;
               --new_val2 := Dac2 + new_val1  + FB;
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
