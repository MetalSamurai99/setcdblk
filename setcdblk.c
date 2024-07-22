/* setcdblk.c	-- Kevin Davidson Tue Nov 14 1995
 *
 * Based on code from Juergen Keil <jk@tools.de>
 *
 * $Log: setcdblk.c,v $
 * Revision 1.2  1997/04/24  12:17:10  tkld
 * Fix error for unknown speeds.
 *
 * Revision 1.1  1997/04/24  11:42:53  tkld
 * Initial revision
 *
 */

#ifndef lint
static char *rcsid = "$Header: /home/tkld/src/cd/setcdblk.c,v 1.2 1997/04/24 12:17:10 tkld Exp $";
#endif

#include <fcntl.h>
#include <sys/cdio.h>
#include <stdlib.h>
#include <stdio.h>

#define VOLDIR "/vol/dev/aliases/"
#define RAWDEV   "/dev/rdsk/c0t6d0s2"
#define DEFLT_BLK 512

main(int argc,char **argv)
{
  int fd, speed, c, flags = O_RDONLY|O_NDELAY | O_EXCL;
  int block_size = DEFLT_BLK;
  char *cdarg = RAWDEV;
  char *cdrom;
  extern char *optarg;
  extern int optind;
  int verbose=0, setspeed=0;

  while ((c=getopt(argc,argv,"b:d:s:v")) != EOF)
    switch (c) {
    case 'b':
      block_size = atoi(optarg);
      break;
    case 'v':
      verbose++;
      break;
    case 's':
      setspeed++;
      switch (*optarg) {
      case '1':
	switch (optarg[1]) {
	case '2':
	  speed = CDROM_TWELVE_SPEED;
	  break;
	case '\0':
	  speed = CDROM_NORMAL_SPEED;
	  break;
	default:
	  fprintf(stderr,"%s: unknown speed %s. Should be 1, 2, 4, 12 or m\n",
		  argv[0],optarg);
	  setspeed=0;
	}
	break;
      case '2':
	speed = CDROM_DOUBLE_SPEED;
	break;
      case '4':
	speed = CDROM_QUAD_SPEED;
	break;
      case 'm':
	speed = CDROM_MAXIMUM_SPEED;
	break;
      default:
	fprintf(stderr,"%s: unknown speed %s. Should be 1, 2, 4, 12 or m\n",
		argv[0],optarg);

	setspeed=0;
      }
      break;
    case 'd':
      cdarg = optarg;
      break;
    case '?':
      fprintf(stderr,"Usage: %s [-v] [-d device|alias] [-b blocksize] [-s speed]\n",argv[0]);
      exit(1);
    }

  if (*cdarg != '/') {	/* An alias ? */
    cdrom=malloc(strlen(VOLDIR)+strlen(cdarg)+1);
    strcpy(cdrom,VOLDIR);
    strcat(cdrom,cdarg);
  } else
    cdrom=cdarg;

  fd = open(cdrom, flags);
  if (fd < 0) {
    perror(cdrom);
    exit(1);
  }
  /* Must do this first! */
  if (ioctl(fd, CDROMSBLKMODE, block_size)) {
    perror("CDROMSBLKMODE");
    exit(1);
  }
  if (verbose) {
    printf("%s: block size set to %d\n",cdrom,block_size);
  }
  if (setspeed) {
    if (ioctl(fd,CDROMSDRVSPEED,speed)) {
      perror("CDROMSDRVSPEED");
    }
  }
  if (verbose && !ioctl(fd, CDROMGDRVSPEED,&speed)) {
    printf("%s: ",cdrom);
    switch (speed) {
    case CDROM_NORMAL_SPEED:
      printf("normal speed (1x)\n");
      break;
    case CDROM_DOUBLE_SPEED:
      printf("double speed (2x)\n");
      break;
    case CDROM_QUAD_SPEED:
      printf("quad speed (4x)\n");
      break;
    case CDROM_TWELVE_SPEED:
      printf("twelve speed (12x)\n");
      break;
    case CDROM_MAXIMUM_SPEED:
      printf("maximum speed\n");
      break;
    default:
      printf("unknown %d\n",speed);
    }
  }
  close(fd);
  return 0;
}
