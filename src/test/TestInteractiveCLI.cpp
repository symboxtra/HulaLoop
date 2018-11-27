#include <gtest/gtest.h>

#include <hlaudio/hlaudio.h>
#include <hlcontrol/hlcontrol.h>

#include "CLICommands.h"
#include "InteractiveCLI.h"

using namespace hula;

// Single instance since Transport has to be created
int num = 0;
QCoreApplication app(num, nullptr);
HulaInteractiveCli cli(&app);

#define MOCK_CONTROL_TIME 200

#define OPT_TEST(name, ...)                            \
    std::vector<std::string> args{__VA_ARGS__};        \
                                                       \
    HulaCliStatus r = cli.processCommand(name, args);  \
    HulaSettings *s = HulaSettings::getInstance();     \

/**
 * Utility function for restoring state of CLI.
 */
void restoreState()
{
    std::vector<std::string> args;
    cli.processCommand(HL_STOP_LONG, args);

    args.push_back(HL_DISCARD_ARG1);
    cli.processCommand(HL_DISCARD_LONG, args);
}

/**
 * Send an empty command.
 *
 * EXPECTED:
 *      process returns success and does nothing
 */
TEST(TestInteractiveCLI, empty_line)
{
    OPT_TEST("");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Send a non-existent command.
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, non_existent_command)
{
    OPT_TEST("non_existent");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
}

/**
 * Provide extra args to a command.
 *
 * EXPECTED:
 *      process still returns success
 *      TODO: warning is printed (has not been enabled yet)
 */
TEST(TestInteractiveCLI, extra_args)
{
    OPT_TEST("print", "extra_arg");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Set delay using short command.
 *
 * EXPECTED:
 *      process returns success
 *      delay matches in settings
 */
TEST(TestInteractiveCLI, short_delay)
{
    OPT_TEST(HL_DELAY_TIMER_SHORT, "5.7");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    //EXPECT_EQ(s->getDelayTimer(), 5.7);
}

/**
 * Set delay using long command.
 *
 * EXPECTED:
 *      process returns success
 *      delay matches in settings
 */
TEST(TestInteractiveCLI, long_delay)
{
    OPT_TEST(HL_DELAY_TIMER_LONG, "5.9");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    //EXPECT_EQ(s->getDelayTimer(), 5.9);
}

/**
 * Delay long command with no argument.
 *
 * EXPECTED:
 *      process returns failure
 *      delay remains unchanged
 */
TEST(TestInteractiveCLI, no_arg_long_delay)
{
    OPT_TEST(HL_DELAY_TIMER_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
    //EXPECT_EQ(s->getDelayTimer(), 5.9);
}

/**
 * Delay long command with non-numeric argument.
 *
 * EXPECTED:
 *      process returns failure
 *      delay remains unchanged
 */
TEST(TestInteractiveCLI, NAN_arg_long_delay)
{
    OPT_TEST(HL_DELAY_TIMER_LONG, "not_a_number");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
    //EXPECT_EQ(s->getDelayTimer(), 5.9);
}

/************************************************************/

/**
 * Set duration using short command.
 *
 * EXPECTED:
 *      process returns success
 *      duration matches in settings
 */
TEST(TestInteractiveCLI, short_duration)
{
    OPT_TEST(HL_RECORD_TIMER_SHORT, "5.7");
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    //EXPECT_EQ(s->getRecordDuration(), 5.7);
}

/**
 * Set duration using long command.
 *
 * EXPECTED:
 *      process returns success
 *      duration matches in settings
 */
TEST(TestInteractiveCLI, long_duration)
{
    OPT_TEST(HL_RECORD_TIMER_LONG, "5.9");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    //EXPECT_EQ(s->getRecordDuration(), 5.9);
}

/**
 * Duration long command with no argument.
 *
 * EXPECTED:
 *      process returns failure
 *      duration remains unchanged
 */
TEST(TestInteractiveCLI, no_arg_long_duration)
{
    OPT_TEST(HL_RECORD_TIMER_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
    //EXPECT_EQ(s->getRecordDuration(), 5.9);
}

/**
 * Duration long command with non-numeric argument.
 *
 * EXPECTED:
 *      process returns failure
 *      duration remains unchanged
 */
TEST(TestInteractiveCLI, NAN_arg_long_duration)
{
    OPT_TEST(HL_RECORD_TIMER_LONG, "not_a_number");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
    //EXPECT_EQ(s->getRecordDuration(), 5.9);
}

/************************************************************/

/**
 * Start record.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to RECORD
 */
TEST(TestInteractiveCLI, short_record)
{
    OPT_TEST(HL_RECORD_SHORT);
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::RECORDING);

    restoreState();
}

/**
 * Start record.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to RECORD
 */
TEST(TestInteractiveCLI, long_record)
{
    OPT_TEST(HL_RECORD_LONG);
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::RECORDING);

    restoreState();
}

/**
 * Start record with delay.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to RECORD
 */
TEST(TestInteractiveCLI, delay_long_record)
{
    OPT_TEST(HL_RECORD_LONG, "5.7");
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::RECORDING);

    restoreState();
}

/**
 * Start record with delay and duration.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to RECORD
 */
TEST(TestInteractiveCLI, delay_and_dur_long_record)
{
    OPT_TEST(HL_RECORD_LONG, "5.7", "-1");
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::RECORDING);

    restoreState();
}

/**
 * Start record with non-numeric delay.
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, NAN_delay_long_record)
{
    OPT_TEST(HL_RECORD_LONG, "not_a_number");
    std::this_thread::sleep_for(std::chrono::milliseconds(MOCK_CONTROL_TIME));

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);

    restoreState();
}

/**
 * Start record with normal delay and non-numeric duration.
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, delay_and_NAN_dur_long_record)
{
    OPT_TEST(HL_RECORD_LONG, "not_a_number", "not_a_number");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);

    restoreState();
}

/**
 * Start record with non-numeric delay and duration.
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, NAN_delay_and_dur_long_record)
{
    OPT_TEST(HL_RECORD_LONG, "5.9", "not_a_number");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);

    restoreState();
}

/************************************************************/

/**
 * Stop record.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to STOPPED
 */
TEST(TestInteractiveCLI, DISABLED_short_stop)
{
    OPT_TEST(HL_STOP_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::STOPPED);

    restoreState();
}

/**
 * Stop record.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to STOPPED
 */
TEST(TestInteractiveCLI, DISABLED_long_stop)
{
    OPT_TEST(HL_STOP_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::STOPPED);

    restoreState();
}

/************************************************************/

/**
 * Pause record.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to PAUSED
 */
TEST(TestInteractiveCLI, DISABLED_short_pause)
{
    OPT_TEST(HL_PAUSE_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::PAUSED);

    restoreState();
}

/**
 * Pause record.
 *
 * EXPECTED:
 *      process returns success
 *      Transport state switches to PAUSED
 */
TEST(TestInteractiveCLI, DISABLED_long_pause)
{
    OPT_TEST(HL_PAUSE_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
    EXPECT_EQ(cli.getState(), TransportState::PAUSED);

    restoreState();
}

/************************************************************/

/**
 * Export with no argument.
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, no_arg_long_export)
{
    OPT_TEST(HL_EXPORT_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
}

/************************************************************/

/**
 * Set input device with no argument.
 *
 * This MUST come before select_by_id_long_input
 * until we stop using setting's defaultInputDevice
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, no_arg_long_input_select)
{
    OPT_TEST(HL_INPUT_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
}

/**
 * DISABLED:
 *      Can't guarentee that device 0 will be
 *      capable of input on all systems.
 *
 * Set input device using ID.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, DISABLED_select_by_id_long_input)
{
    OPT_TEST(HL_INPUT_LONG, "0");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Set input to non-existent device.
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, select_non_existent_long_input)
{
    OPT_TEST(HL_INPUT_LONG, "non_existent");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
}

/************************************************************/

/**
 * Set ouput device with no argument.
 *
 * This MUST come before select_by_id_long_output
 * until we stop using setting's defaultOutputDevice
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, no_arg_long_ouput_select)
{
    OPT_TEST(HL_OUTPUT_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
}

/**
 * DISABLED:
 *      Can't guarentee that device 0 will be
 *      capable of output on all systems.
 *
 * Set ouput device using ID.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, DISABLED_select_by_id_long_output)
{
    OPT_TEST(HL_OUTPUT_LONG, "0");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Set ouput to non-existent device.
 *
 * EXPECTED:
 *      process returns failure
 */
TEST(TestInteractiveCLI, select_non_existent_long_output)
{
    OPT_TEST(HL_OUTPUT_LONG, "non_existent");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_FAILURE);
}

/************************************************************/

/**
 * List devices using short command.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, short_list)
{
    OPT_TEST(HL_LIST_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * List devices using long command.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, long_list)
{
    OPT_TEST(HL_LIST_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * List devices when RECORD devices are set
 * as hidden.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, list_devices_when_record_devices_are_hidden)
{
    HulaSettings *set = HulaSettings::getInstance();
    set->setShowRecordDevices(false);

    OPT_TEST(HL_LIST_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/************************************************************/

/**
 * Print settings.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, short_print)
{
    OPT_TEST(HL_PRINT_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Print settings.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, long_print)
{
    OPT_TEST(HL_PRINT_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/************************************************************/

/**
 * Print version using short option.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, short_version)
{
    OPT_TEST(HL_VERSION_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Print version using long option.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, long_version)
{
    OPT_TEST(HL_VERSION_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/************************************************************/

/**
 * Print help.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, short_help)
{
    OPT_TEST(HL_HELP_SHORT);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Print help.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, long_help)
{
    OPT_TEST(HL_HELP_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/************************************************************/

/**
 * Run sys command.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, short_sys)
{
    OPT_TEST(HL_SYSTEM_SHORT, "dir");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Run sys command.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, long_sys)
{
    OPT_TEST(HL_SYSTEM_LONG, "dir");

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/**
 * Run sys command with no args.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, no_args_long_sys)
{
    OPT_TEST(HL_SYSTEM_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_SUCCESS);
}

/************************************************************/

/**
 * Exit.
 *
 * EXPECTED:
 *      process returns success
 */
TEST(TestInteractiveCLI, long_exit)
{
    OPT_TEST(HL_EXIT_LONG);

    ASSERT_EQ(r, HulaCliStatus::HULA_CLI_EXIT);
}

/************************************************************/
