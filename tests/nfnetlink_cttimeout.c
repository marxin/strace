/*
 * Copyright (c) 2017 JingPiao Chen <chenjingpiao@gmail.com>
 * Copyright (c) 2017-2018 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "tests.h"

#ifdef HAVE_LINUX_NETFILTER_NFNETLINK_CTTIMEOUT_H

# include <stdio.h>
# include <sys/socket.h>
# include "netlink.h"
# include <linux/netfilter/nfnetlink.h>
# include <linux/netfilter/nfnetlink_cttimeout.h>

static void
test_nlmsg_type(const int fd)
{
	long rc;
	struct nlmsghdr nlh = {
		.nlmsg_len = sizeof(nlh),
		.nlmsg_flags = NLM_F_REQUEST,
	};

	nlh.nlmsg_type =
		NFNL_SUBSYS_CTNETLINK_TIMEOUT << 8 | IPCTNL_MSG_TIMEOUT_NEW;
	rc = sendto(fd, &nlh, nlh.nlmsg_len, MSG_DONTWAIT, NULL, 0);
	printf("sendto(%d, {nlmsg_len=%u"
	       ", nlmsg_type=NFNL_SUBSYS_CTNETLINK_TIMEOUT<<8"
	       "|IPCTNL_MSG_TIMEOUT_NEW"
	       ", nlmsg_flags=NLM_F_REQUEST, nlmsg_seq=0, nlmsg_pid=0}"
	       ", %u, MSG_DONTWAIT, NULL, 0) = %s\n",
	       fd, nlh.nlmsg_len, nlh.nlmsg_len, sprintrc(rc));

	nlh.nlmsg_type = NFNL_SUBSYS_CTNETLINK_TIMEOUT << 8 | 0xff;
	rc = sendto(fd, &nlh, nlh.nlmsg_len, MSG_DONTWAIT, NULL, 0);
	printf("sendto(%d, {nlmsg_len=%u"
	       ", nlmsg_type=NFNL_SUBSYS_CTNETLINK_TIMEOUT<<8|0xff"
	       " /* IPCTNL_MSG_TIMEOUT_??? */"
	       ", nlmsg_flags=NLM_F_REQUEST, nlmsg_seq=0, nlmsg_pid=0}"
	       ", %u, MSG_DONTWAIT, NULL, 0) = %s\n",
	       fd, nlh.nlmsg_len, nlh.nlmsg_len, sprintrc(rc));
}

static void
test_nlmsg_flags(const int fd)
{
	long rc;
	struct nlmsghdr nlh = {
		.nlmsg_len = sizeof(nlh),
	};

	nlh.nlmsg_type =
		NFNL_SUBSYS_CTNETLINK_TIMEOUT << 8 | IPCTNL_MSG_TIMEOUT_NEW;
	nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_APPEND;
	rc = sendto(fd, &nlh, nlh.nlmsg_len, MSG_DONTWAIT, NULL, 0);
	printf("sendto(%d, {nlmsg_len=%u"
	       ", nlmsg_type=NFNL_SUBSYS_CTNETLINK_TIMEOUT<<8"
	       "|IPCTNL_MSG_TIMEOUT_NEW"
	       ", nlmsg_flags=NLM_F_REQUEST|NLM_F_APPEND, nlmsg_seq=0"
	       ", nlmsg_pid=0}, %u, MSG_DONTWAIT, NULL, 0) = %s\n",
	       fd, nlh.nlmsg_len, nlh.nlmsg_len, sprintrc(rc));

	nlh.nlmsg_type =
		NFNL_SUBSYS_CTNETLINK_TIMEOUT << 8 | IPCTNL_MSG_TIMEOUT_GET;
	nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_ATOMIC;
	rc = sendto(fd, &nlh, nlh.nlmsg_len, MSG_DONTWAIT, NULL, 0);
	printf("sendto(%d, {nlmsg_len=%u"
	       ", nlmsg_type=NFNL_SUBSYS_CTNETLINK_TIMEOUT<<8"
	       "|IPCTNL_MSG_TIMEOUT_GET"
	       ", nlmsg_flags=NLM_F_REQUEST|NLM_F_ATOMIC, nlmsg_seq=0"
	       ", nlmsg_pid=0}, %u, MSG_DONTWAIT, NULL, 0) = %s\n",
	       fd, nlh.nlmsg_len, nlh.nlmsg_len, sprintrc(rc));

	nlh.nlmsg_type =
		NFNL_SUBSYS_CTNETLINK_TIMEOUT << 8 | IPCTNL_MSG_TIMEOUT_DELETE;
	nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_NONREC;
	rc = sendto(fd, &nlh, nlh.nlmsg_len, MSG_DONTWAIT, NULL, 0);
	printf("sendto(%d, {nlmsg_len=%u"
	       ", nlmsg_type=NFNL_SUBSYS_CTNETLINK_TIMEOUT<<8"
	       "|IPCTNL_MSG_TIMEOUT_DELETE"
	       ", nlmsg_flags=NLM_F_REQUEST|NLM_F_NONREC, nlmsg_seq=0"
	       ", nlmsg_pid=0}, %u, MSG_DONTWAIT, NULL, 0) = %s\n",
	       fd, nlh.nlmsg_len, nlh.nlmsg_len, sprintrc(rc));
}

int
main(void)
{
	skip_if_unavailable("/proc/self/fd/");

	int fd = create_nl_socket(NETLINK_NETFILTER);

	test_nlmsg_type(fd);
	test_nlmsg_flags(fd);

	puts("+++ exited with 0 +++");

	return 0;
}

#else

SKIP_MAIN_UNDEFINED("HAVE_LINUX_NETFILTER_NFNETLINK_CTTIMEOUT_H")

#endif
