
entity andg is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1, in2 : bit := '0';
          out1 : out bit := '0');               
end andg;

architecture only of andg is
begin

    p1: process(in1, in2)
        variable val,ex_value : bit := '0';
    begin
        val := in1 and in2;

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;


------------------------------------------------------------------------
-- or gate
------------------------------------------------------------------------

entity org is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1, in2 : bit := '0';
          out1 : out bit := '0');
end org;

architecture only of org is
begin

    p1: process(in1, in2)
        variable val,ex_value : bit := '0';
    begin
        val := in1 or in2;

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;


------------------------------------------------------------------------
-- xor gate
------------------------------------------------------------------------

entity xorg is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1, in2 : bit := '0';
          out1 : out bit := '0');
end xorg;

architecture only of xorg is
begin

    p1: process(in1, in2)
        variable val,ex_value : bit := '0';
    begin
        val := (in1 and not(in2)) or (not(in1) and in2);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;


------------------------------------------------------------------------
-- xnor gate
------------------------------------------------------------------------

entity xnorg is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1, in2 : bit := '0';
          out1 : out bit := '0');

end xnorg;

architecture only of xnorg is
begin

    p1: process(in1, in2)
        variable val,ex_value : bit := '0';
    begin
        val := (not(in1) and not(in2)) or (in1 and in2);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;



------------------------------------------------------------------------
-- nand gate
------------------------------------------------------------------------

entity nandg is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1, in2 : bit := '0';
          out1 : out bit := '0');

end nandg;

architecture only of nandg is
begin

    p1: process(in1, in2)
        variable val,ex_value : bit := '0';
    begin
        val := not(in1 and in2);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;



------------------------------------------------------------------------
-- nor gate
------------------------------------------------------------------------

entity norg is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1, in2 : bit := '0';
          out1 : out bit := '0');

end norg;

architecture only of norg is
begin

    p1: process(in1, in2)
        variable val,ex_value : bit := '0';
    begin
        val := not(in1 or in2);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;


------------------------------------------------------------------------
-- inv gate
------------------------------------------------------------------------

entity invg is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1: bit := '0';
          out1 : out bit := '0');

end invg;

architecture only of invg is
begin

    p1: process(in1)
        variable val,ex_value : bit := '0';

    begin
        val := not(in1);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;


------------------------------------------------------------------------
-- buff gate
------------------------------------------------------------------------

entity buffg is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1: bit := '0';
          out1 : out bit := '0');

end buffg;

architecture only of buffg is
begin

    p1: process(in1)
        variable val,ex_value : bit := '0';

    begin
        val := in1;

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;


--***************** PORTES GENERIQ0ES S0R LES ENTREES

------------------------------------------------------------------------
-- and gate generic on input number
------------------------------------------------------------------------

entity andg_n is
    generic (n      : integer := 2;
             tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (inp : bit_vector(0 to n-1) := (others => '0');
          out1 : out bit := '0'
          );               
end andg_n;

architecture only of andg_n is
begin

    p1: process(inp)   
        variable val,ex_value : bit := '0';
    begin
        val:='1';
        for i in inp'range loop
          val := val and inp(i);
        end loop;
          
        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;

entity andg_3 is
   generic (tpd_hl : time := 1 fs;
            tpd_lh : time := 1 fs);
   port    (in1,in2,in3 : in bit;
            out1 : out bit);
end andg_3;

architecture only of andg_3 is
begin
p1:
 out1 <= in1 and in2 and in3 after tpd_hl;

end only;

entity andg_4 is
   generic (tpd_hl : time := 1 fs;
            tpd_lh : time := 1 fs);
   port    (in1,in2,in3,in4 : in bit;
            out1 : out bit);
end andg_4;

architecture only of andg_4 is
begin
p1:
 out1 <= in1 and in2 and in3 and in4 after tpd_hl;

end only;

entity andg_5 is
   generic (tpd_hl : time := 1 fs;
            tpd_lh : time := 1 fs);
   port    (in1,in2,in3,in4,in5 : in bit;
            out1 : out bit);
end andg_5;

architecture only of andg_5 is
begin
p1:
 out1 <= in1 and in2 and in3 and in4 and in5 after tpd_hl;

end only;


entity andg_8 is
   generic (tpd_hl : time := 1 fs;
            tpd_lh : time := 1 fs);
   port    (in1,in2,in3,in4,in5,in6,in7,in8 : in bit;
            out1 : out bit);
end andg_8;

architecture only of andg_8 is
begin
p1:
 out1 <= in1 and in2 and in3 and in4 and in5 and in6 and in7
         and in8 after tpd_hl;

end only;


entity andg_9 is
   generic (tpd_hl : time := 1 fs;
            tpd_lh : time := 1 fs);
   port    (in1,in2,in3,in4,in5,in6,in7,in8,in9 : in bit;
            out1 : out bit);
end andg_9;

architecture only of andg_9 is
begin
p1:
 out1 <= in1 and in2 and in3 and in4 and in5 and in6 and in7 and in8
         and in9 after tpd_hl;

end only;
------------------------------------------------------------------------
-- nand gate generic on input number
------------------------------------------------------------------------

entity nandg_n is
    generic (n      : integer :=2;
             tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (inp : bit_vector(0 to n-1) := (others => '0');
          out1 : out bit := '0'
          );               
end nandg_n;

architecture only of nandg_n is
begin

    p1: process(inp)   
        variable val,ex_value : bit := '0';
    begin
        val:='1';
        for i in inp'range loop
          val := val and inp(i);
        end loop;
        val := not (val);
             
        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;

entity nandg_3 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3 : in bit;
          out1 : out bit
         );
end nandg_3;

architecture only of nandg_3 is
begin
p1:
 out1 <= not(in1 and in2 and in3) after tpd_hl;

end only;

entity nandg_4 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3,in4 : in bit;
          out1 : out bit
          );
end nandg_4;

architecture only of nandg_4 is
begin
p1:
 out1 <= not(in1 and in2 and in3 and in4) after tpd_hl;

end only;

entity nandg_5 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3,in4,in5 : in bit;
          out1 : out bit
          );
end nandg_5;

architecture only of nandg_5 is
begin
p1:
 out1 <= not(in1 and in2 and in3 and in4 and in5) after tpd_hl;

end only;

entity nandg_8 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3,in4,in5,in6,in7,in8 : in bit;
          out1 : out bit
          );
end nandg_8;

architecture only of nandg_8 is
begin
p1:
 out1 <= not(in1 and in2 and in3 and in4 and in5 and in6 and in7 
             and in8) after tpd_hl;

end only;

------------------------------------------------------------------------
-- or gate generic on input number
------------------------------------------------------------------------

entity org_n is
    generic (n      : integer :=2;
             tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (inp : bit_vector(0 to n-1) := (others => '0');
          out1 : out bit := '0'
          );               
end org_n;

architecture only of org_n is
begin

    p1: process(inp)   
        variable val,ex_value : bit := '0';
    begin
        val:='0';
        for i in inp'range loop
          val := val or inp(i);
        end loop;
          
        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;

entity org_3 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3 : bit;
          out1 : out bit
          );
end org_3;

architecture only of org_3 is
begin
p1:
 out1 <= in1 or in2 or in3 after tpd_hl;

end only;

entity org_4 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3,in4 : bit;
          out1 : out bit
          );
end org_4;

architecture only of org_4 is
begin
p1:
 out1 <= in1 or in2 or in3 or in4 after tpd_hl;

end only;

entity org_5 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3,in4,in5 : bit;
          out1 : out bit
          );
end org_5;

architecture only of org_5 is
begin
p1:
 out1 <= in1 or in2 or in3 or in4 or in5 after tpd_hl;

end only;


------------------------------------------------------------------------
-- nor gate generic on input number
------------------------------------------------------------------------

entity norg_n is
    generic (n      : integer :=2;
             tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (inp : bit_vector(0 to n-1) := (others => '0');
          out1 : out bit := '0'
          );               
end norg_n;

architecture only of norg_n is
begin

    p1: process(inp)   
        variable val,ex_value : bit := '0';
    begin
        val:='0';
        for i in inp'range loop
          val := val or inp(i);
        end loop;
        val := not(val);
          
        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;

entity norg_3 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3 : bit;
          out1 : out bit := '0'
          );
end norg_3;

architecture only of norg_3 is
begin
p1: 
  out1 <= not(in1 or in2 or in3) after tpd_hl;

end only;

entity norg_4 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3,in4 : bit;
          out1 : out bit := '0'
          );
end norg_4;

architecture only of norg_4 is
begin
p1:
  out1 <= not(in1 or in2 or in3 or in4) after tpd_hl;

end only;

entity norg_8 is
    generic (tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (in1,in2,in3,in4,in5,in6,in7,in8 : bit := '0';
          out1 : out bit := '0'
          );
end norg_8;

architecture only of norg_8 is
begin
p1: 
  out1 <= not(in1 or in2 or in3 or in4 or in5 or in6 or in7 
              or in8) after tpd_hl;

end only;

------------------------------------------------------------------------
-- xor gate generic on input number
------------------------------------------------------------------------

entity xorg_n is
    generic (n      : integer :=2;
             tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (inp : bit_vector(0 to n-1) := (others => '0');
          out1 : out bit := '0'
          );               
end xorg_n;

architecture only of xorg_n is
begin

    p1: process(inp)   
        variable val,ex_value : bit := '0';
    begin
        val:='0';
        for i in inp'range loop
          val := (val and not(inp(i))) or (not(val) and inp(i));
        end loop;
          
        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;




------------------------------------------------------------------------
-- xnor gate generic on input number
------------------------------------------------------------------------

entity xnorg_n is
    generic (n      : integer :=2;
             tpd_hl : time := 1 fs;
             tpd_lh : time := 1 fs);
    port (inp : bit_vector(0 to n-1) := (others => '0');
          out1 : out bit := '0'
          );               
end xnorg_n;

architecture only of xnorg_n is
begin

    p1: process(inp)   
        variable val,ex_value : bit := '0';
    begin
        val:='0';
        for i in inp'range loop
          val := (val and not(inp(i))) or (not(val) and inp(i));
        end loop;
        val := not(val);
          
        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                out1 <= val after tpd_hl;
            when '1' =>
                out1 <= val after tpd_lh;
            when others =>
          end case;
        end if;  
    end process;

end only;

