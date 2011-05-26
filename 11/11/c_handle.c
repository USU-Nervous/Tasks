unsigned short *video = (unsigned short *)0xB8000;
unsigned short magic = 0xE020;
void c_handler()
{
	video[0] = magic;
	video++;
	if (video > 0xB809F){
		magic += 0x1000;
		video = 0xB8000;
	}
}
