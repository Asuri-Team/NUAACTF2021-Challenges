#!/bin/bash
# echo $FLAG > /app/flag
echo "NUAACTF{test_falg}" > /app/flag
export FLAG=not_flag
FLAG=not_flag

crystal build /app/main.cr --release -o /app/main
cd /app && chmod +x main && /app/main