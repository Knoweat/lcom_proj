#ifndef __RTC_CONSTS_H
#define __RTC_CONSTS_H

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_IRQ 8

enum
{
	SECONDS_R,
	SECONDS_ALARM_R,
	MINUTES_R,
	MINUTES_ALARM_R,
	HOURS_R,
	HOURS_ALARM_R,
	DAY_WEEK_R,
	DAY_MONTH_R,
	MONTH_R,
	YEAR_R,
	REG_A,
	REG_B,
	REG_C,
	REG_D
};

/*
 * @brief Register A bits
 */
#define UIP BIT(7)
#define DV2 BIT(6)
#define DV1 BIT(5)
#define DV0 BIT(4)
#define RS3 BIT(3)
#define RS2 BIT(2)
#define RS1 BIT(1)
#define RS0 BIT(0)

/*
 * @brief Register B bits
 */
#define SET BIT(7)
#define PIE BIT(6)
#define AIE BIT(5)
#define UIE BIT(4)
#define SQWE BIT(3)
#define DM BIT(2)
#define HR BIT(1)
#define DSE BIT(0)

/*
 * @brief Register C bits
 */
#define IRQF BIT(7)
#define PF BIT(6)
#define AF BIT(5)
#define UF BIT(4)

/*
 * @brief Register D bit
 */
#define VRT BIT(7)

typedef enum { alarm_int, update_int, periodic_int }rtc_int_t;

#endif
