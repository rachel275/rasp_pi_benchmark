To install the blis library:
1. Run updates
```bash
  sudo apt update
  sudo apt install build-essential git
```
2. Clone blis
   ```bash
   git clone https://github.com/flame/blis.git
   cd blis
    ```
3. Configure blis: On the Raspberry Pi we can use the automcatic feature that will detect the CPU and memory
```bash
./configure auto
```
4. Make
   ```bash
   make -j$(nproc)
    ```

5. Install - this will install by default to /usr/local/lib
```bash
sudo make install
```
