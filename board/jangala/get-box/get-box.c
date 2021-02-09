// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2015-2016 Wills Wang <wills.wang@live.com>
 */
#include <common.h>
#include <init.h>
#include <log.h>
#include <asm/io.h>
#include <asm/addrspace.h>
#include <asm/types.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <mach/ath79.h>
#include <mach/ar71xx_regs.h>
#include <mach/ddr.h>
#include <debug_uart.h>

static void get_box_usb_start(void)
{
	u32 bootstrap;
	u32 val;
	void __iomem *pregs = map_physmem(AR71XX_PLL_BASE, AR71XX_PLL_SIZE,
					  MAP_NOCACHE);

	val = readl(pregs + QCA953X_PLL_SWITCH_CLOCK_CONTROL_REG);
	val &= ~(((1 << (8)) - 1) << 4);

	bootstrap = ath79_get_bootstrap();
	if (bootstrap & QCA953X_BOOTSTRAP_REF_CLK_40) {
		log_info("Serial clock is 40MHz\n");
		val |= (0x5 << 8);
	}
	else {
		log_info("Serial clock is 25MHz\n");
		val |= (0x2 << 8);
	}
	log_info("Writing %u value to register.\n", val);
	writel(val, pregs + QCA953X_PLL_SWITCH_CLOCK_CONTROL_REG);
	mdelay(10);
}

#ifdef CONFIG_DEBUG_UART_BOARD_INIT
void board_debug_uart_init(void)
{
	void __iomem *regs;
	u32 val;

	regs = map_physmem(AR71XX_GPIO_BASE, AR71XX_GPIO_SIZE,
			   MAP_NOCACHE);

	/*
	 * GPIO9 as input, GPIO10 as output
	 */
	val = readl(regs + AR71XX_GPIO_REG_OE);
	val |= QCA953X_GPIO(9);
	val &= ~QCA953X_GPIO(10);
	writel(val, regs + AR71XX_GPIO_REG_OE);

	/*
	 * Enable GPIO10 as UART0_SOUT
	 */
	val = readl(regs + QCA953X_GPIO_REG_OUT_FUNC2);
	val &= ~QCA953X_GPIO_MUX_MASK(16);
	val |= QCA953X_GPIO_OUT_MUX_UART0_SOUT << 16;
	writel(val, regs + QCA953X_GPIO_REG_OUT_FUNC2);

	/*
	 * Enable GPIO9 as UART0_SIN
	 */
	val = readl(regs + QCA953X_GPIO_REG_IN_ENABLE0);
	val &= ~QCA953X_GPIO_MUX_MASK(8);
	val |= QCA953X_GPIO_IN_MUX_UART0_SIN << 8;
	writel(val, regs + QCA953X_GPIO_REG_IN_ENABLE0);

	/*
	 * Enable GPIO10 output
	 */
	val = readl(regs + AR71XX_GPIO_REG_OUT);
	val |= QCA953X_GPIO(10);
	writel(val, regs + AR71XX_GPIO_REG_OUT);
}
#endif

int board_early_init_f(void)
{
	ddr_init();
	ath79_eth_reset();
	get_box_usb_start();
	//ath79_eth_reset();
	return 0;
}
