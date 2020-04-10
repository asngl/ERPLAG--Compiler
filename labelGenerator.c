LabelGenerator *initLabelGenerator()
{
	LabelGenerator *lg=(LabelGenerator *)malloc(sizeof(LabelGenerator));
	strcpy(lg->prefix,".LABEL");
	lg->counter=0;
}