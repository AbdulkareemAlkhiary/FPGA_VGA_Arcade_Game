module kbd(ar, clk, ps2_clk, ps2_dat, key_value, code_rdy);
	input	ar;		// active low
	input	clk;  // 25 or 50 MHz should be okay
	input  ps2_clk;
	input	ps2_dat;
	output reg [3:0] key_value; // 0, 1, 3,.., A, B,... F (hex values)
	output reg  code_rdy;
	
	reg [7:0] code;
	   
	reg	ps2_clk_filt;
	reg [7:0] filter_sr;
	
	always @(negedge ar or posedge clk)
	if(~ar)
	   begin
			ps2_clk_filt = 1'b0;
			filter_sr = 8'b0;
	   end
	else
	   begin
	   filter_sr = {ps2_clk, filter_sr[7:1]};
	   
	   if(filter_sr == 8'hff)
		ps2_clk_filt = 1'b1;
		
		else if(filter_sr == 8'h00)
			ps2_clk_filt = 1'b0;
			
	  end
    
    
	reg [3:0]	bit_count;
	reg			currently_receiving;
	
	always @(negedge ar or posedge ps2_clk_filt)
		if(~ar)
			begin
				bit_count = 0;
				code_rdy = 1'b0;
				code = 7'h00;
				currently_receiving = 1'b0;
				
			end
		else
			begin
				if(~currently_receiving && ps2_dat == 1'b0)	// Indicates current receiving a start bit_count	
					begin
						currently_receiving = 1'b1;
						bit_count = 0;
						code_rdy = 1'b0;
					end
				else
					begin
					   if(currently_receiving)	
						begin
						
						bit_count = bit_count + 1'b1;
						
						if(bit_count <= 4'd8)
							code = {ps2_dat, code[7:1]}; // Shift in the latest ps2 data bit_count
						else	
							begin
								code_rdy = 1'b1;
								currently_receiving = 1'b0;
							end
						end
					end
			end
			
							
	
	always@(code)
		case(code)						// These are the numbers along the top row of the keyboard
			8'h1C: key_value = 4'hA;
			8'h1D: key_value = 4'hB;
			8'h1B: key_value = 4'hC;
			8'h23: key_value = 4'hD;
			8'h29: key_value = 4'hE;
			8'h49: key_value = 4'hF;
			8'h45: key_value = 4'h0;
			8'h16: key_value = 4'h1;
			8'h1E: key_value = 4'h2;
			8'h26: key_value = 4'h3;
			8'h25: key_value = 4'h4;
			8'h2E: key_value = 4'h5;
			8'h36: key_value = 4'h6;
			8'h3D: key_value = 4'h7;
			8'h3E: key_value = 4'h8;
			8'h46: key_value = 4'h9;
			default: key_value = 4'h0;
		endcase
	
					
	endmodule
	