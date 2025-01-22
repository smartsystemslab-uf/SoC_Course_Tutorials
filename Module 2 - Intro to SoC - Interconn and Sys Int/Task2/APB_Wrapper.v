`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/29/2021 08:46:16 PM
// Design Name: 
// Module Name: apb_wrapper_policy_server
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module APB_Wrapper#(
    parameter BUS_WIDTH = 32,
    parameter ADDR_OFFSET= 'h00000000)
(   input clk,
    input rst,
    // Interface to APB bridge
    input S_PSEL,
    input S_PENABLE,
    input [31:0] S_PADDR,
    input S_PWRITE,
    input [31:0] S_PWDATA,
    output reg  S_PREADY,
    output reg  S_PSLVERR,
    output reg [31:0] S_PRDATA);
    
    wire apb_write;
    wire apb_read;
    wire [5:0] addr_index;

    // Begin user wires and regs
    reg ce_i;
    reg [7:0] a_i,b_i;
    wire [15:0] p_o;
    // End user wires and regs
    
    assign apb_write = S_PSEL & S_PENABLE & S_PWRITE;
    assign apb_read = S_PSEL & ~S_PWRITE;
    assign addr_index = S_PADDR[6:0];
   

    // Begin user module instantiation
    mult_booth_array # (
             .word_size_a(8),
             .word_size_b(8),
             .sync_in_out(0),
             .use_pipelining(1)) 
			 mult_booth_array_inst(
             .clk_i(clk),  .rst_i(!rst),
             .ce_i(ce_i),    //in
             .a_i(a_i),           //in
             .b_i(b_i),           //in
             .p_o(p_o)         //out
			 );
    // End user module instantiation
     
    always@(posedge clk)
    begin
        if (!rst) begin
            S_PREADY  <= 1;
            S_PSLVERR <= 0;
            S_PRDATA  <= 0;
        end
        else begin
        S_PREADY  <= 0;
        S_PSLVERR <= 0;
            
            if (apb_write) begin
            case (addr_index)
                    5'h00: ce_i   <= S_PWDATA;
                    5'h04: a_i    <= S_PWDATA;
                    5'h08: b_i    <= S_PWDATA;
            endcase
            S_PREADY  <= 1;
            end 
            else if (apb_read) begin
				case (addr_index)
					5'h00: S_PRDATA <= ce_i;
					5'h04: S_PRDATA <= a_i;
					5'h08: S_PRDATA <= b_i;
					5'h0C: S_PRDATA <= p_o;
				endcase
				S_PREADY  <= 1;
			end
			else   
				S_PRDATA  <= 0;          
        end
    end
endmodule

