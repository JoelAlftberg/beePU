#pragma once

#include <cstdint>

static constexpr std::uint8_t REG_WIDTH_BITS{16U};
static constexpr std::uint8_t INSTRUCTION_WIDTH{2U};
static constexpr std::uint32_t RAM_SIZE{0x10000};
static constexpr std::uint8_t BYTE_WIDTH{8U};
static constexpr std::uint8_t REG_AMOUNT{16U};

static constexpr std::uint8_t TWO_REG_OP{0b00};
static constexpr std::uint8_t IMMEDIATE_OP{0b01};
static constexpr std::uint8_t JUMP_OP{0b10};
static constexpr std::uint8_t SINGLE_REG_OP{0b11};