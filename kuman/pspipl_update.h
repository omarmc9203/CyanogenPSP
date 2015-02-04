#ifndef __PSPIPL_UPDATE_H__
#define __PSPIPL_UPDATE_H__

/**
 * Gets the ipl from nand
 *
 * @param buf - The buffer where the ipl will be stored
 *
 * @returns - the size of ipl on success, < 0 on error
*/
int pspIplUpdateGetIpl(u8 *buf);

#endif


