CURDIR := $(abspath .)
BPFDIR := $(CURDIR)/bpf
DPPROG := $(BPFDIR)/sched_ext
GOBPFDIR := $(CURDIR)/internal/bpf

BPFTOOL := bpftool
CLANG := clang
GO := go
RM := rm

## check if the vmlinux exists in /sys/kernel/btf directory
VMLINUX_BTF ?= $(wildcard /sys/kernel/btf/vmlinux)
ifeq ($(VMLINUX_BTF),)
$(error Cannot find a vmlinux)
endif

LDFLAGS := -ldflags='-extldflags "-static"' -buildvcs=false 
bin/scx_simple: $(GO_SOURCES) vmlinux build-bpf
	@$(GO) build $(LDFLAGS) -o $@ .

.PHONY: vmlinux
vmlinux: $(BPFDIR)/vmlinux.h

.PHONY: build-bpf
build-bpf:
	@$(GO) generate main.go

$(BPFDIR)/vmlinux.h:
	@$(BPFTOOL) btf dump file $(VMLINUX_BTF) format c > $@

.PHONY: clean
clean:
	-@$(RM) -f $(BPFDIR)/vmlinux.h
	-@$(RM) -f ./*.o
	-@$(RM) -f ./bpf_x86_*.go
