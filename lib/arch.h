#pragma once

#include <cstdint>

/* General */
static constexpr std::uint8_t BYTE_WIDTH{8U};
static constexpr std::uint16_t LOWER_BYTE_MASK{0x00FF};

/* Display */
static constexpr std::uint16_t DISPLAY_PANE_COLS{80U};
static constexpr std::uint16_t DISPLAY_PANE_LINES{25U};

static constexpr std::uint16_t RIGHT_PANE_COLS{23U};

static constexpr std::uint16_t STATUS_PANE_LINES{21U};
static constexpr std::uint16_t MEMORY_PANE_LINES{21U};

static constexpr std::uint16_t TOTAL_HEIGHT{STATUS_PANE_LINES + MEMORY_PANE_LINES};
static constexpr std::uint16_t TOTAL_WIDTH{DISPLAY_PANE_COLS + RIGHT_PANE_COLS};

static constexpr std::uint16_t RIGHT_PANE_LINES{TOTAL_HEIGHT};

/* Registers */
static constexpr std::uint8_t REG_WIDTH_BITS{16U};
static constexpr std::uint8_t REG_AMOUNT{16U};
static constexpr std::uint8_t BANK_REGISTER{10U};
static constexpr std::uint8_t SCRATCH_REGISTER{0U};
static constexpr std::uint8_t STACK_POINTER_REGISTER{15U};

/* Instruction */
static constexpr std::uint8_t INSTRUCTION_WIDTH{2U};

static constexpr std::uint8_t FORMAT_SHIFT{14U};
static constexpr std::uint8_t OPCODE_SHIFT{10U};
static constexpr std::uint8_t SRC_SHIFT{4U};
static constexpr std::uint8_t DST_SHIFT{0U};

static constexpr std::uint8_t TWO_REG_OP{0b00};
static constexpr std::uint8_t IMMEDIATE_OP{0b01};
static constexpr std::uint8_t JUMP_OP{0b10};
static constexpr std::uint8_t SINGLE_REG_OP{0b11};

/* Memory banks */
static constexpr std::uint8_t NUM_BANKS{16U};
static constexpr std::uint8_t DEFAULT_BANK{0U};
static constexpr std::uint8_t STACK_BANK_INDEX{1U};

/* Memory mapping */

/* Fixed RAM map*/
static constexpr std::uint16_t FIXED_REGION_START{0x0000};
static constexpr std::uint16_t FIXED_REGION_END{0x7FFF};
static constexpr std::size_t FIXED_REGION_SIZE{0x8000};

/* Bank map */
static constexpr std::uint16_t BANKED_REGION_START{0x8000};
static constexpr std::uint16_t BANKED_REGION_END{0xDFFF};
static constexpr std::size_t BANK_MEM_SIZE{0x6000};

/* Display map */
static constexpr std::uint16_t DISPLAY_REGION_START{0xE000};
static constexpr std::uint16_t DISPLAY_REGION_END{0xE7FF};
static constexpr std::size_t DISPLAY_MEM_SIZE{0x800};

/* IO map*/
static constexpr std::uint16_t IO_REGION_START{0xE800};
static constexpr std::uint16_t IO_REGION_END{0xEFFF};
static constexpr std::size_t IO_MEM_SIZE{0x800};

/* ROM map*/
static constexpr std::uint16_t ROM_REGION_START{0xF000};
static constexpr std::uint16_t ROM_REGION_END{0xFFFF};
static constexpr std::size_t ROM_SIZE{0x1000};
