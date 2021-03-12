-- Copyright (C) 2019  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions 
-- and other software and tools, and any partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Intel Program License 
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details, at
-- https://fpgasoftware.intel.com/eula.

-- VENDOR "Altera"
-- PROGRAM "Quartus Prime"
-- VERSION "Version 19.1.0 Build 670 09/22/2019 SJ Lite Edition"

-- DATE "11/10/2020 11:03:31"

-- 
-- Device: Altera 10M50DAF484C7G Package FBGA484
-- 

-- 
-- This VHDL file should be used for ModelSim-Altera (VHDL) only
-- 

LIBRARY FIFTYFIVENM;
LIBRARY IEEE;
USE FIFTYFIVENM.FIFTYFIVENM_COMPONENTS.ALL;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY 	hard_block IS
    PORT (
	devoe : IN std_logic;
	devclrn : IN std_logic;
	devpor : IN std_logic
	);
END hard_block;

-- Design Ports Information
-- ~ALTERA_TMS~	=>  Location: PIN_H2,	 I/O Standard: 3.3 V Schmitt Trigger,	 Current Strength: Default
-- ~ALTERA_TCK~	=>  Location: PIN_G2,	 I/O Standard: 3.3 V Schmitt Trigger,	 Current Strength: Default
-- ~ALTERA_TDI~	=>  Location: PIN_L4,	 I/O Standard: 3.3 V Schmitt Trigger,	 Current Strength: Default
-- ~ALTERA_TDO~	=>  Location: PIN_M5,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
-- ~ALTERA_CONFIG_SEL~	=>  Location: PIN_H10,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
-- ~ALTERA_nCONFIG~	=>  Location: PIN_H9,	 I/O Standard: 3.3 V Schmitt Trigger,	 Current Strength: Default
-- ~ALTERA_nSTATUS~	=>  Location: PIN_G9,	 I/O Standard: 3.3 V Schmitt Trigger,	 Current Strength: Default
-- ~ALTERA_CONF_DONE~	=>  Location: PIN_F8,	 I/O Standard: 3.3 V Schmitt Trigger,	 Current Strength: Default


ARCHITECTURE structure OF hard_block IS
SIGNAL gnd : std_logic := '0';
SIGNAL vcc : std_logic := '1';
SIGNAL unknown : std_logic := 'X';
SIGNAL ww_devoe : std_logic;
SIGNAL ww_devclrn : std_logic;
SIGNAL ww_devpor : std_logic;
SIGNAL \~ALTERA_TMS~~padout\ : std_logic;
SIGNAL \~ALTERA_TCK~~padout\ : std_logic;
SIGNAL \~ALTERA_TDI~~padout\ : std_logic;
SIGNAL \~ALTERA_CONFIG_SEL~~padout\ : std_logic;
SIGNAL \~ALTERA_nCONFIG~~padout\ : std_logic;
SIGNAL \~ALTERA_nSTATUS~~padout\ : std_logic;
SIGNAL \~ALTERA_CONF_DONE~~padout\ : std_logic;
SIGNAL \~ALTERA_TMS~~ibuf_o\ : std_logic;
SIGNAL \~ALTERA_TCK~~ibuf_o\ : std_logic;
SIGNAL \~ALTERA_TDI~~ibuf_o\ : std_logic;
SIGNAL \~ALTERA_CONFIG_SEL~~ibuf_o\ : std_logic;
SIGNAL \~ALTERA_nCONFIG~~ibuf_o\ : std_logic;
SIGNAL \~ALTERA_nSTATUS~~ibuf_o\ : std_logic;
SIGNAL \~ALTERA_CONF_DONE~~ibuf_o\ : std_logic;

BEGIN

ww_devoe <= devoe;
ww_devclrn <= devclrn;
ww_devpor <= devpor;
END structure;


LIBRARY ALTERA;
LIBRARY FIFTYFIVENM;
LIBRARY IEEE;
USE ALTERA.ALTERA_PRIMITIVES_COMPONENTS.ALL;
USE FIFTYFIVENM.FIFTYFIVENM_COMPONENTS.ALL;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY 	differentialController_simulation IS
    PORT (
	Q_locked : OUT std_logic;
	locked : IN std_logic;
	limitedSlip : IN std_logic;
	auto : IN std_logic;
	clock : IN std_logic;
	Q_limitedSlip : OUT std_logic;
	Q_auto : OUT std_logic;
	Q_enFlipFlop : OUT std_logic;
	Q_en_CLK : OUT std_logic
	);
END differentialController_simulation;

-- Design Ports Information
-- Q_locked	=>  Location: PIN_J11,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 8mA
-- Q_limitedSlip	=>  Location: PIN_C9,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 8mA
-- Q_auto	=>  Location: PIN_A9,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 8mA
-- Q_enFlipFlop	=>  Location: PIN_A7,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 8mA
-- Q_en_CLK	=>  Location: PIN_A8,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 8mA
-- locked	=>  Location: PIN_B11,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
-- limitedSlip	=>  Location: PIN_H12,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
-- auto	=>  Location: PIN_B8,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
-- clock	=>  Location: PIN_B10,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default


ARCHITECTURE structure OF differentialController_simulation IS
SIGNAL gnd : std_logic := '0';
SIGNAL vcc : std_logic := '1';
SIGNAL unknown : std_logic := 'X';
SIGNAL devoe : std_logic := '1';
SIGNAL devclrn : std_logic := '1';
SIGNAL devpor : std_logic := '1';
SIGNAL ww_devoe : std_logic;
SIGNAL ww_devclrn : std_logic;
SIGNAL ww_devpor : std_logic;
SIGNAL ww_Q_locked : std_logic;
SIGNAL ww_locked : std_logic;
SIGNAL ww_limitedSlip : std_logic;
SIGNAL ww_auto : std_logic;
SIGNAL ww_clock : std_logic;
SIGNAL ww_Q_limitedSlip : std_logic;
SIGNAL ww_Q_auto : std_logic;
SIGNAL ww_Q_enFlipFlop : std_logic;
SIGNAL ww_Q_en_CLK : std_logic;
SIGNAL \~QUARTUS_CREATED_ADC1~_CHSEL_bus\ : std_logic_vector(4 DOWNTO 0);
SIGNAL \~QUARTUS_CREATED_ADC2~_CHSEL_bus\ : std_logic_vector(4 DOWNTO 0);
SIGNAL \inst63~clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \~QUARTUS_CREATED_GND~I_combout\ : std_logic;
SIGNAL \~QUARTUS_CREATED_UNVM~~busy\ : std_logic;
SIGNAL \~QUARTUS_CREATED_ADC1~~eoc\ : std_logic;
SIGNAL \~QUARTUS_CREATED_ADC2~~eoc\ : std_logic;
SIGNAL \Q_locked~output_o\ : std_logic;
SIGNAL \Q_limitedSlip~output_o\ : std_logic;
SIGNAL \Q_auto~output_o\ : std_logic;
SIGNAL \Q_enFlipFlop~output_o\ : std_logic;
SIGNAL \Q_en_CLK~output_o\ : std_logic;
SIGNAL \locked~input_o\ : std_logic;
SIGNAL \clock~input_o\ : std_logic;
SIGNAL \limitedSlip~input_o\ : std_logic;
SIGNAL \auto~input_o\ : std_logic;
SIGNAL \inst63~combout\ : std_logic;
SIGNAL \inst63~clkctrl_outclk\ : std_logic;
SIGNAL \inst44~combout\ : std_logic;
SIGNAL \inst6~feeder_combout\ : std_logic;
SIGNAL \inst6~q\ : std_logic;
SIGNAL \inst43~combout\ : std_logic;
SIGNAL \inst11~feeder_combout\ : std_logic;
SIGNAL \inst11~q\ : std_logic;
SIGNAL \inst27~0_combout\ : std_logic;
SIGNAL \inst27~1_combout\ : std_logic;
SIGNAL \inst27~2_combout\ : std_logic;
SIGNAL \inst36~0_combout\ : std_logic;

COMPONENT hard_block
    PORT (
	devoe : IN std_logic;
	devclrn : IN std_logic;
	devpor : IN std_logic);
END COMPONENT;

BEGIN

Q_locked <= ww_Q_locked;
ww_locked <= locked;
ww_limitedSlip <= limitedSlip;
ww_auto <= auto;
ww_clock <= clock;
Q_limitedSlip <= ww_Q_limitedSlip;
Q_auto <= ww_Q_auto;
Q_enFlipFlop <= ww_Q_enFlipFlop;
Q_en_CLK <= ww_Q_en_CLK;
ww_devoe <= devoe;
ww_devclrn <= devclrn;
ww_devpor <= devpor;

\~QUARTUS_CREATED_ADC1~_CHSEL_bus\ <= (\~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\);

\~QUARTUS_CREATED_ADC2~_CHSEL_bus\ <= (\~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\ & \~QUARTUS_CREATED_GND~I_combout\);

\inst63~clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \inst63~combout\);
auto_generated_inst : hard_block
PORT MAP (
	devoe => ww_devoe,
	devclrn => ww_devclrn,
	devpor => ww_devpor);

-- Location: LCCOMB_X44_Y52_N16
\~QUARTUS_CREATED_GND~I\ : fiftyfivenm_lcell_comb
-- Equation(s):
-- \~QUARTUS_CREATED_GND~I_combout\ = GND

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000000000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	combout => \~QUARTUS_CREATED_GND~I_combout\);

-- Location: IOOBUF_X49_Y54_N23
\Q_locked~output\ : fiftyfivenm_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \inst27~0_combout\,
	devoe => ww_devoe,
	o => \Q_locked~output_o\);

-- Location: IOOBUF_X46_Y54_N16
\Q_limitedSlip~output\ : fiftyfivenm_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \inst27~1_combout\,
	devoe => ww_devoe,
	o => \Q_limitedSlip~output_o\);

-- Location: IOOBUF_X46_Y54_N23
\Q_auto~output\ : fiftyfivenm_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \inst27~2_combout\,
	devoe => ww_devoe,
	o => \Q_auto~output_o\);

-- Location: IOOBUF_X49_Y54_N30
\Q_enFlipFlop~output\ : fiftyfivenm_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \inst36~0_combout\,
	devoe => ww_devoe,
	o => \Q_enFlipFlop~output_o\);

-- Location: IOOBUF_X46_Y54_N2
\Q_en_CLK~output\ : fiftyfivenm_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \inst63~combout\,
	devoe => ww_devoe,
	o => \Q_en_CLK~output_o\);

-- Location: IOIBUF_X49_Y54_N8
\locked~input\ : fiftyfivenm_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	listen_to_nsleep_signal => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_locked,
	o => \locked~input_o\);

-- Location: IOIBUF_X46_Y54_N8
\clock~input\ : fiftyfivenm_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	listen_to_nsleep_signal => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_clock,
	o => \clock~input_o\);

-- Location: IOIBUF_X49_Y54_N15
\limitedSlip~input\ : fiftyfivenm_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	listen_to_nsleep_signal => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_limitedSlip,
	o => \limitedSlip~input_o\);

-- Location: IOIBUF_X46_Y54_N29
\auto~input\ : fiftyfivenm_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	listen_to_nsleep_signal => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_auto,
	o => \auto~input_o\);

-- Location: LCCOMB_X45_Y53_N12
inst63 : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst63~combout\ = LCELL((\clock~input_o\ & (\locked~input_o\ $ (\limitedSlip~input_o\ $ (\auto~input_o\)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1000010001001000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \locked~input_o\,
	datab => \clock~input_o\,
	datac => \limitedSlip~input_o\,
	datad => \auto~input_o\,
	combout => \inst63~combout\);

-- Location: CLKCTRL_G14
\inst63~clkctrl\ : fiftyfivenm_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \inst63~clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \inst63~clkctrl_outclk\);

-- Location: LCCOMB_X45_Y53_N10
inst44 : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst44~combout\ = (\auto~input_o\) # (\locked~input_o\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111110101010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \auto~input_o\,
	datad => \locked~input_o\,
	combout => \inst44~combout\);

-- Location: LCCOMB_X45_Y53_N14
\inst6~feeder\ : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst6~feeder_combout\ = \inst44~combout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111000011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \inst44~combout\,
	combout => \inst6~feeder_combout\);

-- Location: FF_X45_Y53_N15
inst6 : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \inst63~clkctrl_outclk\,
	d => \inst6~feeder_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \inst6~q\);

-- Location: LCCOMB_X45_Y53_N22
inst43 : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst43~combout\ = (\auto~input_o\) # (\limitedSlip~input_o\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101011111010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \auto~input_o\,
	datac => \limitedSlip~input_o\,
	combout => \inst43~combout\);

-- Location: LCCOMB_X45_Y53_N26
\inst11~feeder\ : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst11~feeder_combout\ = \inst43~combout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111000011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \inst43~combout\,
	combout => \inst11~feeder_combout\);

-- Location: FF_X45_Y53_N27
inst11 : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \inst63~clkctrl_outclk\,
	d => \inst11~feeder_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \inst11~q\);

-- Location: LCCOMB_X45_Y53_N4
\inst27~0\ : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst27~0_combout\ = (\inst6~q\ & !\inst11~q\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \inst6~q\,
	datac => \inst11~q\,
	combout => \inst27~0_combout\);

-- Location: LCCOMB_X46_Y53_N24
\inst27~1\ : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst27~1_combout\ = (\inst11~q\ & !\inst6~q\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \inst11~q\,
	datad => \inst6~q\,
	combout => \inst27~1_combout\);

-- Location: LCCOMB_X46_Y53_N18
\inst27~2\ : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst27~2_combout\ = (\inst11~q\ & \inst6~q\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111000000000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \inst11~q\,
	datad => \inst6~q\,
	combout => \inst27~2_combout\);

-- Location: LCCOMB_X45_Y53_N18
\inst36~0\ : fiftyfivenm_lcell_comb
-- Equation(s):
-- \inst36~0_combout\ = \auto~input_o\ $ (\limitedSlip~input_o\ $ (\locked~input_o\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1010010101011010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \auto~input_o\,
	datac => \limitedSlip~input_o\,
	datad => \locked~input_o\,
	combout => \inst36~0_combout\);

-- Location: UNVM_X0_Y40_N40
\~QUARTUS_CREATED_UNVM~\ : fiftyfivenm_unvm
-- pragma translate_off
GENERIC MAP (
	addr_range1_end_addr => -1,
	addr_range1_offset => -1,
	addr_range2_end_addr => -1,
	addr_range2_offset => -1,
	addr_range3_offset => -1,
	is_compressed_image => "false",
	is_dual_boot => "false",
	is_eram_skip => "false",
	max_ufm_valid_addr => -1,
	max_valid_addr => -1,
	min_ufm_valid_addr => -1,
	min_valid_addr => -1,
	part_name => "quartus_created_unvm",
	reserve_block => "true")
-- pragma translate_on
PORT MAP (
	nosc_ena => \~QUARTUS_CREATED_GND~I_combout\,
	xe_ye => \~QUARTUS_CREATED_GND~I_combout\,
	se => \~QUARTUS_CREATED_GND~I_combout\,
	busy => \~QUARTUS_CREATED_UNVM~~busy\);

-- Location: ADCBLOCK_X43_Y52_N0
\~QUARTUS_CREATED_ADC1~\ : fiftyfivenm_adcblock
-- pragma translate_off
GENERIC MAP (
	analog_input_pin_mask => 0,
	clkdiv => 1,
	device_partname_fivechar_prefix => "none",
	is_this_first_or_second_adc => 1,
	prescalar => 0,
	pwd => 1,
	refsel => 0,
	reserve_block => "true",
	testbits => 66,
	tsclkdiv => 1,
	tsclksel => 0)
-- pragma translate_on
PORT MAP (
	soc => \~QUARTUS_CREATED_GND~I_combout\,
	usr_pwd => VCC,
	tsen => \~QUARTUS_CREATED_GND~I_combout\,
	chsel => \~QUARTUS_CREATED_ADC1~_CHSEL_bus\,
	eoc => \~QUARTUS_CREATED_ADC1~~eoc\);

-- Location: ADCBLOCK_X43_Y51_N0
\~QUARTUS_CREATED_ADC2~\ : fiftyfivenm_adcblock
-- pragma translate_off
GENERIC MAP (
	analog_input_pin_mask => 0,
	clkdiv => 1,
	device_partname_fivechar_prefix => "none",
	is_this_first_or_second_adc => 2,
	prescalar => 0,
	pwd => 1,
	refsel => 0,
	reserve_block => "true",
	testbits => 66,
	tsclkdiv => 1,
	tsclksel => 0)
-- pragma translate_on
PORT MAP (
	soc => \~QUARTUS_CREATED_GND~I_combout\,
	usr_pwd => VCC,
	tsen => \~QUARTUS_CREATED_GND~I_combout\,
	chsel => \~QUARTUS_CREATED_ADC2~_CHSEL_bus\,
	eoc => \~QUARTUS_CREATED_ADC2~~eoc\);

ww_Q_locked <= \Q_locked~output_o\;

ww_Q_limitedSlip <= \Q_limitedSlip~output_o\;

ww_Q_auto <= \Q_auto~output_o\;

ww_Q_enFlipFlop <= \Q_enFlipFlop~output_o\;

ww_Q_en_CLK <= \Q_en_CLK~output_o\;
END structure;


