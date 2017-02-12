# Apex_Simulator
Computer Architecture Apex Simulator. Implemented a cycle by cycle simulator for the simple in order 5 stage pipeline.

Pipeline stages.

Fetch: 
Select next instruction from program store. 

Decode:  
Look up register source operands from register file. 
Resolve conditional branches. Command. Fetch to perform branch (and any necessary squashing). 

Execute:
Perform arithmetic. Compute memory address. 

Memory:
Reader write data	memory. 

Writeback: 
Store arithmetic	or LOAD	resulting to register file.

Required	instructions:	
• ADD	<dest	register>,	<register>,	<register	or	literal>	
• SUB	<dest	register>,	<register>,	<register	or	literal>	
• AND	<dest	register>,	<register>,	<register	or	literal>	
• OR	<dest	register>,	<register>,	<register	or	literal>	
• XOR	<dest	register>,	<register>,	<register	or	literal>	
• MOVC	<dest	register>,	<literal>	
• LOAD	<data	register>,	<address	register>,	<literal	offset>	
• STORE	<data	register>,	<address	register>,	<literal	offset>	
• BZ	<literal	offset>	
• BNZ	<literal	offset>	
• JUMP	<absolute	instruction	address>	
• BAL	<absolute	instruction	address>
• Halt	
• NOP	
