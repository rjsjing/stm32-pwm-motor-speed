# CLAUDE.md — STM32F103C8T6 PWM电机转速控制

## 技术栈

- **MCU**: STM32F103C8T6（Cortex-M3, 64KB Flash, 20KB SRAM, 72MHz）
- **库**: ST 标准外设库 v3.5.0
- **构建工具**: Keil MDK uVision5 + EIDE (VSCode 插件 `cl.eide`)
- **调试器**: ST-Link (SWD)

## 目录结构

```
├── Start/           CMSIS 启动 + system_stm32f10x
├── Library/         ST 标准外设库 v3.5.0
├── System/          Delay.c/h — SysTick 延时
├── Hardwera/        自定义硬件驱动
│   ├── Motor.c/h    ← 直流电机驱动 (PA4/PA5 方向 + PA2 PWM)
│   ├── PWM.c/h      ← TIM2 PWM 底层驱动
│   ├── Key.c/h      ← 按键驱动
│   └── OLED.c/h     ← OLED 显示驱动
├── User/            main.c, stm32f10x_conf.h, stm32f10x_it.c/h
└── build/Objects/Listings/
```

## 硬件引脚映射

| 引脚 | 功能 | 连接目标 |
|------|------|----------|
| PA4 | GPIO Out PP | 电机方向 A (正转高) |
| PA5 | GPIO Out PP | 电机方向 B (反转高) |
| PA2 | TIM2_CH3 AF PP | 电机 PWM 调速 |
| PB1 | GPIO In IPU | 按键1 |
| PB11 | GPIO In IPU | 按键2 |
| PB5 | GPIO Out OD | OLED SCL |
| PB6 | GPIO Out OD | OLED SDA |

## 驱动模块

### Motor (Hardwera/Motor.c/h)
- `Motor_Init()` — 初始化方向引脚 (PA4/PA5) + PWM
- `Motor_SetSpeed(Speed)` — 设置速度 (-100~100)
  - 正数: PA4高/PA5低, PWM 占空比 = Speed%
  - 负数: PA4低/PA5高, PWM 占空比 = |Speed|%

### PWM (Hardwera/PWM.c/h)
- `PWM_Init()` — TIM2 CH3 PWM, 频率 1kHz
- `PWM_SetCompare3(Compare)` — 设置 CCR3

## 功能说明

按键控制直流电机转速: 每次按下按键1 速度+20 (0→20→40→...→100→0)。OLED 显示当前速度值。正转速=正转, 负转速对应反向但本代码仅实现正转。

## 编码规范
- 函数命名: `模块名_操作()`
- 编码: UTF-8，注释用中文

## Git 规范
```
main              ← 稳定分支
提交: feat:/fix:/docs: 格式
```

## 构建命令
```
Keil:  打开 Project.uvprojx → F7 编译 → F8 烧录
EIDE:  Ctrl+Shift+P → "EIDE: Build"
```

## 注意事项
- `USE_STDPERIPH_DRIVER` + `STM32F10X_MD` 必须定义
- 启动文件 `startup_stm32f10x_md.s`
- 电机驱动需外接 TB6612/L9110S 等电机驱动模块
