#!/bin/sh
# ============================================
# QNX Startup Script
# Project: QNX RTOS Temperature Monitoring
# ============================================

echo "Starting QNX RTOS Temperature Monitoring System..."

# Ensure binaries are executable
chmod +x /bin/logger_process
chmod +x /bin/sensor_process
chmod +x /bin/cli_process

# Start logger first (server)
echo "Starting Logger Process..."
/bin/logger_process &

sleep 2

# Start sensor
echo "Starting Sensor Process..."
/bin/sensor_process &

sleep 1

# Start CLI
echo "Starting CLI Process..."
/bin/cli_process &

echo "All processes started successfully."

