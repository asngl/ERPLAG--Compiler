f= open("sample.asm","r+")
out = open("gen.txt","w+")
contents=f.readlines()
for line in contents:
	text='fprintf(fp,"'+str(line[:-1])+'");\n';
	if(text != "\n"):
		out.write(text)

