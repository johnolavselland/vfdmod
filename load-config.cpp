#include <QFile>
#include <QSettings>
#include <QStringList>
#include "default-values.h"
#include "structures.h"
#include <QDebug>

extern int checkFlag;
extern QString exeName;

/*
 * QString::toInt(bool *ok, int base = 10) is not available in Qt4,
 * so I will use my own converter from a hex string to an integer.
 */
int hex_to_int(QString s, bool *ok)
{
    int base = 1;
    int result = 0;
    QString digits = "0123456789abcdef";

    if (s.toLower().startsWith("0x"))
        s.remove(0, 2);

    if (s.isEmpty()) {
        *ok = false;
        return -1;
    }

    for (int i = s.length() - 1; i >= 0; --i) {
        *ok = false;
        for (int j = 0; j < digits.length(); ++j) {
            if (s.toLower().at(i) == digits.at(j)) {
                *ok = true;
                result += j * base;
                continue;
            }
        }
        if (!*ok)
            return -1;
        base *= 16;
    }

    return result;
}

int load_main_group(QSettings &ini, main_config_t &mconfig)
{
    if (checkFlag)
        printf("[%s]\n", qPrintable(GROUP_MAIN));

    ini.beginGroup(GROUP_MAIN);
    mconfig.componentName = ini.value(KEY_COMPONENT_NAME, exeName).toString();
    ini.endGroup();

    if (mconfig.componentName.isEmpty())
        mconfig.componentName = exeName;

    if(checkFlag)
        printf("%s\t: %s\n", KEY_COMPONENT_NAME, qPrintable(mconfig.componentName));

    return 0;
}

int load_rs485_group(QSettings &ini, rs485_config_t &rs485)
{
    bool ok;
    QString key;

    ini.beginGroup(GROUP_RS485);
    if (checkFlag)
        printf("\n[%s]\n", qPrintable(GROUP_RS485));

    /* Slave address */
    rs485.slaveAddress = ini.value(key = KEY_SLAVE_ADDRESS, VALUE_SLAVE_ADDRESS).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((rs485.slaveAddress < 0) || (rs485.slaveAddress > 0xFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_SLAVE_ADDRESS, rs485.slaveAddress);

    /* Serial device path */
    rs485.serialDevice = ini.value(key = KEY_SERIAL_DEVICE, VALUE_SERIAL_DEVICE).toString();
    if (rs485.serialDevice.isEmpty())
        goto fail_invalid_parameter;
    if (checkFlag)
        printf("%s\t: %s\n", KEY_SERIAL_DEVICE, qPrintable(rs485.serialDevice));

    /* Baud rate */
    rs485.baudRate = ini.value(key = KEY_BAUD_RATE, VALUE_BAUD_RATE).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (rs485.baudRate < 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_BAUD_RATE, rs485.baudRate);

    /* Data bits */
    rs485.dataBits = ini.value(key = KEY_DATA_BITS, VALUE_DATA_BITS).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (rs485.dataBits < 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_DATA_BITS, rs485.dataBits);

    /* Parity */
    rs485.parity = ini.value(key = KEY_PARITY, VALUE_PARITY).toString();
    if (rs485.parity.isEmpty())
        goto fail_invalid_parameter;
    if ((rs485.parity != "N")
            && (rs485.parity != "E")
            && (rs485.parity != "O"))
        goto fail_invalid_parameter;
    if (checkFlag)
        printf("%s\t\t: %s\n", KEY_PARITY, qPrintable(rs485.parity));

    /* Stop bits */
    rs485.stopBits = ini.value(key = KEY_STOP_BITS, VALUE_STOP_BITS).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (rs485.stopBits < 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_STOP_BITS, rs485.stopBits);

    /* Loop delay in milliseconds */
    rs485.loopDelayMs = ini.value(key = KEY_LOOP_DELAY_MS, VALUE_LOOP_DELAY_MS).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (rs485.loopDelayMs < 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_LOOP_DELAY_MS, rs485.loopDelayMs);

    /* Protocol delay in characters */
    rs485.protocolDelay = ini.value(key = KEY_PROTOCOL_DELAY, VALUE_PROTOCOL_DELAY).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (rs485.protocolDelay < 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_PROTOCOL_DELAY, rs485.protocolDelay);

    ini.endGroup();
    return 0;
fail_invalid_parameter:
    printf("%s/%s: parameter is wrong or missing!\n", GROUP_RS485, qPrintable(key));
    return -1;
fail_out_of_range:
    printf("%s/%s: parameter is out of range!\n", GROUP_RS485, qPrintable(key));
    return -1;
}

int load_ctrl_group(QSettings &ini, control_config_t &control)
{
    bool ok;
    QString key, value;

    ini.beginGroup(GROUP_CONTROL);
    if (checkFlag)
        printf("\n[%s]\n", qPrintable(GROUP_CONTROL));

    /* Address */
    value = ini.value(key = KEY_ADDRESS, "").toString();
    if (value.toLower().startsWith("0x"))
        control.address = hex_to_int(value, &ok);
    else
        control.address = value.toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((control.address < 0) || (control.address > 0xFFFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %s (%d)\n", KEY_ADDRESS,
               qPrintable(QString("0x%1").arg(control.address, 4, 16, QChar('0'))),
               control.address);

    /* Run forward value */
    value = ini.value(key = KEY_RUN_FWD, "").toString();
    if (value.toLower().startsWith("0x"))
        control.runFwdValue = hex_to_int(value, &ok);
    else
        control.runFwdValue = value.toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((control.runFwdValue < 0) || (control.runFwdValue > 0xFFFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %s (%d)\n", KEY_RUN_FWD,
               qPrintable(QString("0x%1").arg(control.runFwdValue, 4, 16, QChar('0'))),
               control.runFwdValue);

    /* Run reverse value */
    value = ini.value(key = KEY_RUN_REV, "").toString();
    if (value.toLower().startsWith("0x"))
        control.runRevValue = hex_to_int(value, &ok);
    else
        control.runRevValue = value.toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((control.runRevValue < 0) || (control.runRevValue > 0xFFFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %s (%d)\n", KEY_RUN_REV,
               qPrintable(QString("0x%1").arg(control.runRevValue, 4, 16, QChar('0'))),
               control.runRevValue);

    /* Stop value */
    value = ini.value(key = KEY_STOP, "").toString();
    if (value.toLower().startsWith("0x"))
        control.stopValue = hex_to_int(value, &ok);
    else
        control.stopValue = value.toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((control.stopValue < 0) || (control.stopValue > 0xFFFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %s (%d)\n", KEY_STOP,
               qPrintable(QString("0x%1").arg(control.stopValue, 4, 16, QChar('0'))),
               control.stopValue);

    ini.endGroup();
    return 0;
fail_invalid_parameter:
    printf("%s/%s: parameter is wrong or missing!\n", GROUP_CONTROL, qPrintable(key));
    return -1;
fail_out_of_range:
    printf("%s/%s: parameter is out of range!\n", GROUP_CONTROL, qPrintable(key));
    return -1;
}

int load_rpm_in_group(QSettings &ini, spindle_in_config_t &spindle)
{
    bool ok;
    QString key, value;

    ini.beginGroup(GROUP_SPINDLE_IN);
    if (checkFlag)
        printf("\n[%s]\n", qPrintable(GROUP_SPINDLE_IN));

    /* Address */
    value = ini.value(key = KEY_ADDRESS, "").toString();
    if (value.toLower().startsWith("0x"))
        spindle.address = hex_to_int(value, &ok);
    else
        spindle.address = value.toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((spindle.address < 0) || (spindle.address > 0xFFFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %s (%d)\n", KEY_ADDRESS,
               qPrintable(QString("0x%1").arg(spindle.address, 4, 16, QChar('0'))),
               spindle.address);

    /* Multiplier */
    spindle.multiplier = ini.value(key = KEY_MULTIPLIER, VALUE_MULTIPLIER).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (spindle.multiplier <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_MULTIPLIER, spindle.multiplier);

    /* Divider */
    spindle.divider = ini.value(key = KEY_DIVIDER, VALUE_DIVIDER).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (spindle.divider <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %d\n", KEY_DIVIDER, spindle.divider);

    /* Max speed RPM */
    spindle.maxSpeedRpm = ini.value(key = KEY_MAX_SPEED_RPM, "").toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (spindle.maxSpeedRpm <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_MAX_SPEED_RPM, spindle.maxSpeedRpm);

    /* Min speed RPM */
    spindle.minSpeedRpm = ini.value(key = KEY_MIN_SPEED_RPM, "").toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (spindle.minSpeedRpm <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_MIN_SPEED_RPM, spindle.minSpeedRpm);

    ini.endGroup();
    return 0;
fail_invalid_parameter:
    printf("%s/%s: parameter is wrong or missing!\n", GROUP_SPINDLE_IN, qPrintable(key));
    return -1;
fail_out_of_range:
    printf("%s/%s: parameter is out of range!\n", GROUP_SPINDLE_IN, qPrintable(key));
    return -1;
}

int load_rpm_out_group(QSettings &ini, spindle_out_config_t &spindle)
{
    bool ok;
    QString key, value;

    ini.beginGroup(GROUP_SPINDLE_OUT);
    if (checkFlag)
        printf("\n[%s]\n", qPrintable(GROUP_SPINDLE_OUT));

    /* Address */
    value = ini.value(key = KEY_ADDRESS, "").toString();
    if (value.toLower().startsWith("0x"))
        spindle.address = hex_to_int(value, &ok);
    else
        spindle.address = value.toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((spindle.address < 0) || (spindle.address > 0xFFFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %s (%d)\n", KEY_ADDRESS,
               qPrintable(QString("0x%1").arg(spindle.address, 4, 16, QChar('0'))),
               spindle.address);

    /* Multiplier */
    spindle.multiplier = ini.value(key = KEY_MULTIPLIER, VALUE_MULTIPLIER).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (spindle.multiplier <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_MULTIPLIER, spindle.multiplier);

    /* Divider */
    spindle.divider = ini.value(key = KEY_DIVIDER, VALUE_DIVIDER).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (spindle.divider <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %d\n", KEY_DIVIDER, spindle.divider);

    /* At speed threshold */
    spindle.atSpeedThreshold = ini.value(key = KEY_AT_SPEED_THRESHOLD, VALUE_AT_SPEED_THRESHOLD).toDouble(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((spindle.atSpeedThreshold < 0) || (spindle.atSpeedThreshold > 1.0))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s: %f\n", KEY_AT_SPEED_THRESHOLD, spindle.atSpeedThreshold);

    ini.endGroup();
    return 0;
fail_invalid_parameter:
    printf("%s/%s: parameter is wrong or missing!\n", GROUP_SPINDLE_OUT, qPrintable(key));
    return -1;
fail_out_of_range:
    printf("%s/%s: parameter is out of range!\n", GROUP_SPINDLE_OUT, qPrintable(key));
    return -1;
}

int load_user_group(QSettings &ini, const QString &group, QVector<user_config_t> &uconfig)
{
    bool ok;
    QString key, value;
    ini.beginGroup(group);
    if (checkFlag)
        printf("\n[%s]\n", qPrintable(group));

    user_config_t usrcfg;

    /* Address */
    value = ini.value(key = KEY_ADDRESS, "").toString();
    if (value.toLower().startsWith("0x"))
        usrcfg.address = hex_to_int(value, &ok);
    else
        usrcfg.address = value.toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if ((usrcfg.address < 0) || (usrcfg.address > 0xFFFF))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %s (%d)\n", KEY_ADDRESS,
               qPrintable(QString("0x%1").arg(usrcfg.address, 4, 16, QChar('0'))),
               usrcfg.address);

    /* Multiplier */
    usrcfg.multiplier = ini.value(key = KEY_MULTIPLIER, VALUE_MULTIPLIER).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (usrcfg.multiplier <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t: %d\n", KEY_MULTIPLIER, usrcfg.multiplier);

    /* Divider */
    usrcfg.divider = ini.value(key = KEY_DIVIDER, VALUE_DIVIDER).toInt(&ok);
    if (!ok)
        goto fail_invalid_parameter;
    if (usrcfg.divider <= 0)
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %d\n", KEY_DIVIDER, usrcfg.divider);

    /* Pin type */
    value = ini.value(key = KEY_PIN_TYPE, "").toString().toLower();
    if (value.isEmpty())
        goto fail_invalid_parameter;
    if (value == "float")
        usrcfg.pinType = HAL_FLOAT;
    if (value == "s32")
        usrcfg.pinType = HAL_S32;
    if (value == "u32")
        usrcfg.pinType = HAL_U32;
    if ((value != "float")
            && (value != "s32")
            && (value != "u32"))
        goto fail_out_of_range;
    if (checkFlag)
        printf("%s\t\t: %s\n", KEY_PIN_TYPE, qPrintable(value));

    /* Pin name */
    usrcfg.pinName = ini.value(key = KEY_PIN_NAME, "").toString();
    if (usrcfg.pinName.isEmpty())
        goto fail_invalid_parameter;
    if (checkFlag)
        printf("%s\t\t: %s\n", KEY_PIN_NAME, qPrintable(usrcfg.pinName));

    uconfig.append(usrcfg);

    ini.endGroup();
    return 0;
fail_invalid_parameter:
    printf("%s/%s: parameter is wrong or missing!\n", qPrintable(group), qPrintable(key));
    return -1;
fail_out_of_range:
    printf("%s/%s: parameter is out of range!\n", qPrintable(group), qPrintable(key));
    return -1;
}

int load_config(const QString &inifile, main_config_t &mconfig, QVector<user_config_t> &uconfig)
{
    if (!QFile::exists(inifile)) {
        printf("File not found: %s\n", qPrintable(inifile));
        return -1;
    }

    QSettings ini(inifile, QSettings::IniFormat);

    /* Saving all group names to the list */
    QStringList groups = ini.childGroups();

    /* Loading main settings */
    load_main_group(ini, mconfig);
    if (groups.contains(GROUP_MAIN))
        groups.removeOne(GROUP_MAIN);

    /* Loading rs485 settings */
    if (!groups.contains(GROUP_RS485)) {
        printf("Group not found: %s\n", GROUP_RS485);
        return -1;
    } else
        groups.removeOne(GROUP_RS485);

    if (load_rs485_group(ini, mconfig.rs485) < 0)
        return -1;

    /* Loading control settings */
    if (!groups.contains(GROUP_CONTROL)) {
        printf("Group not found: %s\n", GROUP_CONTROL);
        return -1;
    } else
        groups.removeOne(GROUP_CONTROL);

    if (load_ctrl_group(ini, mconfig.control) < 0)
        return -1;

    /* Loading rpm-in settings */
    if (!groups.contains(GROUP_SPINDLE_IN)) {
        printf("Group not found: %s\n", GROUP_SPINDLE_IN);
        return -1;
    } else
        groups.removeOne(GROUP_SPINDLE_IN);

    if (load_rpm_in_group(ini, mconfig.rpmIn) < 0)
        return -1;

    /* Loading rpm-out settings */
    if (!groups.contains(GROUP_SPINDLE_OUT)) {
        printf("Group not found: %s\n", GROUP_SPINDLE_OUT);
        return -1;
    } else
        groups.removeOne(GROUP_SPINDLE_OUT);

    if (load_rpm_out_group(ini, mconfig.rpmOut) < 0)
        return -1;

    /* Loading user settings */
    foreach (QString group, groups) {
        if (load_user_group(ini, group, uconfig) < 0)
            return -1;
    }

    return 0;
}
