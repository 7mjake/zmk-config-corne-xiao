if(CONFIG_CORNE_XIAO_CHARGE_STATUS)
  target_sources(app PRIVATE src/charge_status_reporter.c)
endif()
