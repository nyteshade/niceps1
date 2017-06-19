# niceps1
A nice UNIX prompt that works with MacOS and most flavors of Linux. Note that the design is aimed towards shells or terminals with dark backgrounds. You may wish to alter the prompt colors yourself if you use a light background instead.
Finally, note, that this repository is here primarily for my own usage and I'll help out where I can but wanted to share if others find it useful.

- [x] Shows current IP addresses
- [x] Shows current git and hg branches if you are in such a directory
- [x] Shows current node JS version if you are using node
- [x] Shows user name, host name, and current directory
- [x] Is colorful. :)

![Screenshot](https://github.com/nyteshade/niceps1/raw/master/screenshots/example.png)

# To Install
Move to the directory you wish the script to live in and close the repo

```sh
git clone git@github.com:nyteshade/niceps1.git
```

Then, from the directory containing your niceps1 directory, add a call to source the script from your BASH startup. Other shells work with varying compatibility. 

```sh
printf >>~/.profile "# Add path to your niceps1 checkout\n"
printf >>~/.profile "export PATH=\“${PATH}\”:$(pwd)\n\n"
printf >>~/.profile "# Source the niceps1 script\n"
printf >>~/.profile "source $(pwd)/niceps1\n\n"
```

Finally if you want a more accurate, and likely faster method of detecting the IP addresses on your machine, try building getCIP. You will need to have a C compiler on your machine and it may or may not work depending on your environment. Try typing the following

```sh
# 'buildCIP --view' shows the source code instead of building it
# 'buildCIP <path>' installs the binary to a different path than /usr/local/bin
niceps1 buildCIP
```

followed by the following

```sh
getcip
```

If successful you’ll see something like the following. It lists IP addresses followed by a colon and then by the name of the adapter to which the IP address belongs. niceps1 normally omits the adapter name for brevity and in both cases localhost IPs are automatically omitted.

```
192.168.97.23:en0,10.244.197.209:en3
```





