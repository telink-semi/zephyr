# Repository Rename Policy

## Rule: Never recreate old repository names

The following repository names were renamed under the `telink-semi` GitHub
organization. **Never create a new repository using any of these old names.**

Doing so would permanently break the GitHub 301 redirect that currently sends
all old-URL traffic to the new repository — affecting every developer, CI
pipeline, and external link that still references the old name.

| Old name (DO NOT RECREATE) | Current name | Renamed |
|---|---|---|
| `telink-semi/zephyr` | `telink-semi/tl_zephyr` | 2026-08 |
| `telink-semi/mcuboot` | `telink-semi/tl_mcuboot` | 2026-08 |
| `telink-semi/openthread` | `telink-semi/tl_openthread` | 2026-08 |
| `telink-semi/xz` | `telink-semi/tl_xz` | 2026-08 |
| `telink-semi/openthread_telink_lib` | `telink-semi/tl_openthread_libs` | 2026-09 |
| `telink-semi/connectedhomeip` | `telink-semi/tl_matter` | 2026-08 |

### Why this matters

GitHub's direct-rename feature creates a 301 redirect from the old URL to the
new one. This redirect works **only as long as no repository exists at the old
name**. If someone creates a repo at, for example, `telink-semi/zephyr`, the
redirect is destroyed instantly and cannot be restored by simply deleting the
new repo — all historical links, CI configurations, and clone URLs pointing at
the old name would be permanently broken.

### What to do instead

- Always use the new `tl_*` names for new work, documentation, and CI.
- If you need a repo for testing or experimentation, use a descriptive name
  that does not collide with any old name (e.g. `tl_zephyr_experimental`).
- If you discover a stale reference to an old name in active (non-historical)
  documentation, update it to the new name.
- Historical release notes are exempt — they document the state at release
  time and the 301 redirects cover them.

### Local directory names are unaffected

The repository rename only changes the **remote URL** — the local checkout
directory names do **not** need to change. `west` uses the `path` field in
`west.yml` to determine where each module is checked out, not the remote URL.

For example, `tl_zephyr` is still checked out as `~/zephyrproject/zephyr`
because `west.yml` declares `self.path: zephyr`. Similarly, `tl_mcuboot` is
checked out at `bootloader/mcuboot`, `tl_openthread` at
`modules/lib/openthread`, and `tl_openthread_libs` at
`modules/lib/openthread_telink_lib` — all unchanged.

The same applies to `tl_matter` (formerly `connectedhomeip`): you can clone it
as `git clone https://github.com/telink-semi/tl_matter.git connectedhomeip` and
keep the local directory named `connectedhomeip`.

### 301 redirect status

As of 2026-09, all 301 redirects are active and verified. Old URLs continue to
resolve to the new repositories. There is no plan to deprecate the redirects;
they will remain functional as long as the old names are not reused.
